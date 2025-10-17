// Signal K application template file.
//
// This application demonstrates core SensESP concepts in a very
// concise manner. You can build and upload the application as is
// and observe the value changes on the serial port monitor.
//
// You can use this source file as a basis for your own projects.
// Remove the parts that are not relevant to you, and add your own code
// for external hardware libraries.

#include <memory>

#include "sensesp.h"
#include "sensesp/controllers/smart_switch_controller.h"
#include "sensesp/sensors/analog_input.h"
#include "sensesp/sensors/digital_input.h"
#include "sensesp/sensors/digital_output.h"
#include "sensesp/sensors/sensor.h"
#include "sensesp/signalk/signalk_output.h"
#include "sensesp/signalk/signalk_put_request_listener.h"
#include "sensesp/system/lambda_consumer.h"
#include "sensesp/transforms/repeat.h"
#include "sensesp_app_builder.h"
#include "sensesp/ui/ui_controls.h"
#include "sensesp/ui/config_item.h"
#include "USB.h"
#include "relay_command_handler.h"

//#define M5StackSwitch ;

using namespace sensesp;

#ifdef M5StackSwitch
  const uint8_t kRelayPin1 = 5;
  const uint8_t kRelayPin2 = 6;
#else
// relay GPIO pins
  const uint8_t kRelayPin1 = 1;
  const uint8_t kRelayPin2 = 2;
  const uint8_t kRelayPin3 = 41;
  const uint8_t kRelayPin4 = 42;
  const uint8_t kRelayPin5 = 45;
  const uint8_t kRelayPin6 = 46;
  const uint8_t kBuzzerPin = 21;
  const uint8_t kRGBLedPin = 38;
#endif

RelayCommandHandler* initialize_relay(uint8_t pin, uint8_t index, String name,
                                        String config_path_sk_output) {
  // Initialize the relay pin to output
  pinMode(pin, OUTPUT);
  // Set the relay GPIO pins to LOW (off) initially
  digitalWrite(pin, LOW);
  auto* load_switch = new DigitalOutput(pin);
  String sk_state_path = "electrical.switches." + name + ".state";
  String sk_command_path = "electrical.switches." + name + ".command";
  // Create a switch controller to handle the user press logic and
  // connect it to the load switch...
  SmartSwitchController* controller = new SmartSwitchController(true);
  controller->connect_to(load_switch);

  // In addition to the manual button "click types", a
  // SmartSwitchController accepts explicit state settings via
  // any boolean producer as well as any "truth" values in human readable
  // format via a String producer.
  // Here, we set up a SignalK PUT request listener to handle
  // requests made to the Signal K server to set the switch state.
  // This allows any device on the SignalK network that can make
  // such a request to also control the state of our switch.
  auto* sk_listener = new StringSKPutRequestListener(sk_state_path);
  sk_listener->connect_to(controller->truthy_string_consumer_);

  load_switch->connect_to(new Repeat<bool, bool>(10000))
      ->connect_to(new SKOutputBool(sk_state_path, config_path_sk_output));

  RelayCommandHandler* relayCommandHandler = new RelayCommandHandler(controller);

  auto* sk_command_listener = new StringSKPutRequestListener(sk_command_path);
  sk_command_listener->connect_to(relayCommandHandler);

  relayCommandHandler
      ->connect_to(new SKOutputString(sk_command_path, config_path_sk_output));
  
  return relayCommandHandler;
}

// The setup function performs one-time application initialization.
void setup() {
  SetupSerialDebug(115200);

  // Construct the global SensESPApp() object
  SensESPAppBuilder builder;
  sensesp_app = (&builder)
                    // Set a custom hostname for the app.
                    #ifdef M5StackSwitch
                    ->set_hostname("m5stack-switch")
                    #else
                    ->set_hostname("sensesp-relay6")
                    #endif
                    // Optionally, hard-code the WiFi and Signal K server
                    // settings. This is normally not needed.
                    //->set_wifi_client("My WiFi SSID", "my_wifi_password")
                    //->set_wifi_access_point("My AP SSID", "my_ap_password")
                    //->set_sk_server("192.168.10.3", 80)
                    ->get_app();
#ifdef M5StackSwitch
    // write up everything to Signal K
  auto relay_controller1 = initialize_relay(kRelayPin1, 1,"relay1",
                   "sensesp-relay1");

  auto relay_controller2 = initialize_relay(kRelayPin2, 2, "relay2",
                   "sensesp-relay2");
#else
  // write up everything to Signal K
  auto relay_controller1 = initialize_relay(kRelayPin1, 1,"relay1",
                   "sensesp-relay1");

  auto relay_controller2 = initialize_relay(kRelayPin2, 2, "relay2",
                   "sensesp-relay2");
  auto relay_controller3 = initialize_relay(kRelayPin3, 3, "relay3",
                   "sensesp-relay3");
  auto relay_controller4 = initialize_relay(kRelayPin4, 4, "relay4",
                   "sensesp-relay4");
  auto relay_controller5 = initialize_relay(kRelayPin5, 5, "relay5",
                   "sensesp-relay5");
  auto relay_controller6 = initialize_relay(kRelayPin6, 6, "relay6",
                   "sensesp-relay6");
#endif
    while(true)
    {
      loop();
    }
}

void loop() { event_loop()->tick(); }
