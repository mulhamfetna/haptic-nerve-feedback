#include "ble_service.h"

#include <ArduinoJson.h>
#include <NimBLEDevice.h>

#include "pulse_engine.h"
#include "safety.h"

namespace stim {

static const char* UUID_SVC = "a5f30001-9f6b-4a52-8f2e-1d2c3b4a5e01";
static const char* UUID_PARAMS = "a5f30002-9f6b-4a52-8f2e-1d2c3b4a5e01";
static const char* UUID_CONTROL = "a5f30003-9f6b-4a52-8f2e-1d2c3b4a5e01";
static const char* UUID_STATUS = "a5f30004-9f6b-4a52-8f2e-1d2c3b4a5e01";

static NimBLECharacteristic* g_status = nullptr;
static NimBLEServer* g_server = nullptr;

class ParamsCb : public NimBLECharacteristicCallbacks {
 public:
  explicit ParamsCb(BleService* s) : s_(s) {}
  void onWrite(NimBLECharacteristic* c) override {
    s_->safety_->heartbeat();
    if (s_->engine_->event_running()) return;  // locked during an event

    JsonDocument doc;
    if (deserializeJson(doc, c->getValue().c_str())) return;

    StimParams p = s_->params_;
    if (doc["pw"].is<uint32_t>()) p.pw_us = doc["pw"];
    if (doc["f"].is<uint32_t>()) p.freq_hz = doc["f"];
    if (doc["amp"].is<uint8_t>()) p.amp_pct = doc["amp"];
    if (doc["dur"].is<uint32_t>()) p.dur_ms = doc["dur"];
    if (doc["burst"].is<JsonObject>()) {
      p.burst.n = doc["burst"]["n"] | 0u;
      p.burst.fi_hz = doc["burst"]["fi"] | 200u;
      p.burst.gap_ms = doc["burst"]["gap"] | 37u;
    }
    if (validate(p) == ParamError::OK) s_->params_ = p;
    // (invalid sets are silently dropped; STATUS always shows the active set)
  }

 private:
  BleService* s_;
};

class ControlCb : public NimBLECharacteristicCallbacks {
 public:
  explicit ControlCb(BleService* s) : s_(s) {}
  void onWrite(NimBLECharacteristic* c) override {
    String cmd = String(c->getValue().c_str());
    cmd.trim();
    cmd.toUpperCase();
    s_->safety_->heartbeat();

    if (cmd == "PING") return;
    if (cmd == "ARM") { s_->safety_->arm(); return; }
    if (cmd == "DISARM" || cmd == "STOP") {
      s_->engine_->abort_event();
      if (cmd == "DISARM") s_->safety_->disarm();
      return;
    }
    if (cmd == "GO") { s_->engine_->start_event(s_->params_); return; }
    if (cmd == "MODE WAVE") { s_->engine_->set_mode(OutputMode::WAVE_INTERNAL); return; }
    if (cmd == "MODE GATE") { s_->engine_->set_mode(OutputMode::GATE_EXTERNAL); return; }
  }

 private:
  BleService* s_;
};

void BleService::begin(Safety* safety, PulseEngine* engine) {
  safety_ = safety;
  engine_ = engine;

  NimBLEDevice::init("H1-Stimulator");
  g_server = NimBLEDevice::createServer();
  NimBLEService* svc = g_server->createService(UUID_SVC);

  auto* params = svc->createCharacteristic(UUID_PARAMS, NIMBLE_PROPERTY::WRITE);
  params->setCallbacks(new ParamsCb(this));

  auto* control = svc->createCharacteristic(UUID_CONTROL, NIMBLE_PROPERTY::WRITE);
  control->setCallbacks(new ControlCb(this));

  g_status = svc->createCharacteristic(
      UUID_STATUS, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);

  svc->start();
  NimBLEAdvertising* adv = NimBLEDevice::getAdvertising();
  adv->addServiceUUID(UUID_SVC);
  adv->start();
}

bool BleService::connected() const {
  return g_server && g_server->getConnectedCount() > 0;
}

void BleService::notify_status() {
  if (!g_status) return;
  JsonDocument doc;
  doc["state"] = to_string(safety_->state());
  doc["fault"] = safety_->fault_reason();
  doc["mode"] =
      engine_->mode() == OutputMode::WAVE_INTERNAL ? "wave" : "gate";
  doc["pw"] = params_.pw_us;
  doc["f"] = params_.freq_hz;
  doc["amp"] = params_.amp_pct;
  doc["dur"] = params_.dur_ms;
  if (params_.burst.n > 0) {
    doc["burst"]["n"] = params_.burst.n;
    doc["burst"]["fi"] = params_.burst.fi_hz;
    doc["burst"]["gap"] = params_.burst.gap_ms;
  }
  const Telemetry& t = engine_->telemetry();
  doc["pulses"] = t.pulses_total;
  doc["events"] = t.events_total;
  doc["peak_ma"] = t.last_peak_ma;
  String out;
  serializeJson(doc, out);
  g_status->setValue(out.c_str());
  g_status->notify();
}

}  // namespace stim
