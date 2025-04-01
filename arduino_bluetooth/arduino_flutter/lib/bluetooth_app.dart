import 'dart:convert';
import 'dart:async';
import 'package:flutter/material.dart';
import 'package:flutter_reactive_ble/flutter_reactive_ble.dart';
import 'package:flutter_joystick/flutter_joystick.dart'; // Make sure you have this package imported
import 'dart:math';

// Define UUIDs as constants
const String serviceUUID = "00000000-5EC4-4083-81CD-A10B8D5CF6EC";
const String characteristicUUID = "00000001-5EC4-4083-81CD-A10B8D5CF6EC";

class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key, required this.title});

  final String title;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  final _ble = FlutterReactiveBle();

  StreamSubscription<DiscoveredDevice>? _scanSub;
  StreamSubscription<ConnectionStateUpdate>? _connectSub;
  StreamSubscription<List<int>>? _notifySub;

  List<DiscoveredDevice> _devices = [];
  String? _selectedDeviceId;
  String? _selectedDeviceName;
  var _stateMessage = 'Scanning...';

  QualifiedCharacteristic? _writeCharacteristic;

  bool _isConnected = false;
  Map<String, bool> _buttonFlashing = {};
  Map<String, Timer?> _buttonTimers = {};
  Map<String, bool> _isFlashing = {};
  bool buttonFlashing = false;
  bool _musicOn = false; // Toggle state for music
  bool _platformExtended = false; // Toggle state for platform
  @override
  void initState() {
    super.initState();
    _scanSub = _ble.scanForDevices(withServices: []).listen(_onScanUpdate);
  }

  @override
  void dispose() {
    _notifySub?.cancel();
    _connectSub?.cancel();
    _scanSub?.cancel();
    super.dispose();
  }

  void _onScanUpdate(DiscoveredDevice d) {
    if (d.name.contains("BLE") && !_devices.any((device) => device.id == d.id)) {
      setState(() {
        _devices.add(d);
      });
    }
  }

  void _connectToDevice() {
    if (_selectedDeviceId != null) {
      setState(() {
        _stateMessage = 'Connecting to $_selectedDeviceName...';
      });

      _connectSub = _ble.connectToDevice(id: _selectedDeviceId!).listen(
        (update) {
          if (update.connectionState == DeviceConnectionState.connected) {
            setState(() {
              _stateMessage = 'Connected to $_selectedDeviceName!';
              _isConnected = true;
            });
            _onConnected(_selectedDeviceId!);
          }
        },
        onError: (error) {
          setState(() {
            _stateMessage = 'Connection error: $error';
          });
        },
      );
    }
  }

  void _disconnectFromDevice() {
    try {
      if (_notifySub != null) {
        _notifySub?.cancel();
        _notifySub = null;
      }

      if (_connectSub != null) {
        _connectSub?.cancel();
        _connectSub = null;
      }

      setState(() {
        _isConnected = false;
        _stateMessage = 'Disconnected from $_selectedDeviceName.';
        _writeCharacteristic = null;
      });
    } catch (e) {
      setState(() {
        _stateMessage = 'Error during disconnection: $e';
      });
    }
  }
  void _toggleFlashing(String command) {
  if (command == "STOP") {
    // Stop all flashing buttons
    _buttonTimers.forEach((key, timer) {
      timer?.cancel();
    });
    _buttonTimers.clear();
    _buttonFlashing.clear();
    _isFlashing.clear();
    buttonFlashing = false;
  } else {
    // Stop any currently flashing button before starting a new one
    _buttonTimers.forEach((key, timer) {
      timer?.cancel();
    });
    _buttonTimers.clear();
    _buttonFlashing.clear();
    _isFlashing.clear();
    buttonFlashing = false;

    // Start flashing for the new button
    _buttonFlashing[command] = true;
    _isFlashing[command] = true;
    buttonFlashing = true;

    _buttonTimers[command] = Timer.periodic(
      const Duration(milliseconds: 500),
      (timer) {
        setState(() {
          _isFlashing[command] = !_isFlashing[command]!;
        });
      },
    );
  }
  setState(() {});
}
  void _toggleMusic() {
    setState(() {
      _musicOn = !_musicOn;
      _stateMessage = _musicOn ? "Music turned ON" : "Music turned OFF";
      if (_musicOn) {
        _sendCommand('MUSIC_ON',0);
      } else {
        _sendCommand('MUSIC_OFF',0);
      }
    });
  }

  void _togglePlatform() {
    setState(() {
      _platformExtended = !_platformExtended;
      _stateMessage = _platformExtended 
          ? "Platform EXTENDED" 
          : "Platform SHORTENED";
      // Here you would actually send the appropriate command to your device
      // For example:
      if (_platformExtended) {
        _sendCommand('EXTEND_PLATFORM',0);
      } else {
        _sendCommand('SHORTEN_PLATFORM',0);
      }


    });
  }
  Color _getButtonColor(String command) {
    if (_buttonFlashing[command] == true && _isFlashing[command] == true) {
      return Colors.yellow;
    }
    return Colors.white;
  }

  void _onConnected(String deviceId) {
    final characteristic = QualifiedCharacteristic(
      deviceId: deviceId,
      serviceId: Uuid.parse(serviceUUID),
      characteristicId: Uuid.parse(characteristicUUID),
    );

    _writeCharacteristic = characteristic;

    _notifySub = _ble.subscribeToCharacteristic(characteristic).listen((bytes) {
      setState(() {
        _stateMessage = 'Data received: ${Utf8Decoder().convert(bytes)}';
      });
    });
  }

  Future<void> _sendCommand(String command, double distance) async {
    if (_writeCharacteristic != null) {
      try {
        List<int> messageBytes;
        if(distance != 0)
        {
          String message = '$command,${distance.toStringAsFixed(2)}';
          messageBytes = utf8.encode(message);  // Convert the message to a byte array
        }
        else {
          messageBytes = utf8.encode(command);
        }
          //List<int> messageBytes = utf8.encode(command);
        await _ble.writeCharacteristicWithResponse(
          _writeCharacteristic!,
          value: messageBytes,
        );
        _toggleFlashing(command);
        setState(() {
          _stateMessage = "Command '$command' with distance $distance sent!";
        });
      } catch (e) {
        setState(() {
          _stateMessage = "Error sending command: $e";
        });
      }
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: Text(widget.title),
      ),
      body: Column(
        children: [
          Container(
            padding: const EdgeInsets.all(16.0),
            color: Colors.grey[300],
            width: double.infinity,
            child: Text(
              _stateMessage,
              style: Theme.of(context).textTheme.titleMedium,
              textAlign: TextAlign.center,
            ),
          ),
          if (_devices.isNotEmpty)
            Padding(
              padding: const EdgeInsets.all(16.0),
              child: DropdownButton<String>(
                isExpanded: true,
                hint: const Text("Select a BLE Device"),
                value: _selectedDeviceId,
                items: _devices.map((device) {
                  return DropdownMenuItem(
                    value: device.id,
                    child: Text(device.name),
                  );
                }).toList(),
                onChanged: (value) {
                  setState(() {
                    _selectedDeviceId = value;
                    _selectedDeviceName = _devices
                        .firstWhere((device) => device.id == value)
                        .name;
                  });
                },
              ),
            ),
          if (!_isConnected)
            ElevatedButton(
              onPressed: _selectedDeviceId != null ? _connectToDevice : null,
              child: const Text('Connect'),
            ),
          if (_isConnected)
            ElevatedButton(
              onPressed: _disconnectFromDevice,
              child: const Text('Disconnect'),
            ),
          // **************** command buttons ****************
          Expanded(
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                // Joystick Widget
                Container(
                  width: 200, // Add width for joystick visibility
                  height: 200, // Add height for joystick visibility
                  child: Joystick(
                    base: JoystickBase(
                      decoration: JoystickBaseDecoration(
                        color: Colors.black,
                        drawOuterCircle: false,
                      ),
                      arrowsDecoration: JoystickArrowsDecoration(
                        color: Colors.blue,
                      ),
                    ),
                    listener: (details) {
                      double x = details.x;
                      double y = details.y;
                      double distance = sqrt(x * x + y * y);

                      // Calculate the angle (degrees) from the joystick's X and Y positions
                      double degrees = (180 / 3.14159265359) * (atan2(y, x)); // Convert from radians to degrees
                      // Normalize degrees to the range [0, 360] (clockwise system starting from North)
                      degrees = (degrees + 90) % 360;

                      // Ensure the joystick moves a certain distance before triggering commands
                      //print(distance);
                      if (distance > 0.1) {
                            // Define ranges for directions
                            print(distance);
                            if (degrees >= 0 && degrees < 10) {
                              _sendCommand('FORWARD',distance);
                              print('FORWARD');
                            } else if (degrees >= 10 && degrees < 80) {
                              _sendCommand('FORWARD RIGHT',distance);
                              print('FORWARD RIGHT');
                            } else if (degrees >= 80 && degrees < 100) {
                              _sendCommand('RIGHT',distance);
                              print('RIGHT');
                            } else if (degrees >= 100 && degrees < 170) {
                              _sendCommand('BACKWARDS RIGHT',distance);
                              print('BACKWARDS RIGHT');
                            } else if (degrees >= 170 && degrees < 190) {
                              _sendCommand('BACKWARDS',distance);
                              print('BACKWARDS');
                            } else if (degrees >= 190 && degrees < 260) {
                              _sendCommand('BACKWARDS LEFT',distance);
                              print('BACKWARDS LEFT');
                            } else if (degrees >= 260 && degrees < 280) {
                              _sendCommand('LEFT',distance);
                              print('LEFT');
                            } 
                            else if (degrees >= 280 && degrees < 350) {
                              _sendCommand('FORWARD LEFT',distance);
                              print('FORWARD LEFT');
                            } else {
                              _sendCommand('FORWARD',distance); // For 350 -> 10 degrees
                              print('FORWARD'); // For 350 -> 10 degrees
                            }
                        //print("Joystick position: x = $x, y = $y, angle = $degrees");
                      }
                    },
                  ),
                ),
                const SizedBox(height: 20),
                // **************** other buttons ****************
                Row(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                                        // Music toggle button
                    ElevatedButton(
                                         onPressed: () {
                        if (_isConnected) {
                          if (buttonFlashing) {
                            _sendCommand('STOP',0);
                          } else {
                            _sendCommand('LEFT SIGNAL',0);
                          }
                        }
                      },
                      style: ElevatedButton.styleFrom(
                        backgroundColor: _getButtonColor('LEFT SIGNAL'),
                        foregroundColor: Colors.black,
                      ),
                      
                      child: const Icon(
                        Icons.arrow_left,
                        color: Color(0xFFB8860B),
                        size: 40,
                      ),
                    ),
                   
                    
                    const SizedBox(width: 20),
                    ElevatedButton(
                      onPressed: () {
                        if (_isConnected) {
                          if (buttonFlashing) {
                            _sendCommand('STOP',0);
                          } else {
                            _sendCommand('HAZARD',0);
                          }
                        }
                      },
                      style: ElevatedButton.styleFrom(
                        backgroundColor: _getButtonColor('HAZARD'),
                        foregroundColor: Colors.black,
                      ),
                      child: const Icon(
                        Icons.warning_amber,
                        color: Colors.red,
                        size: 40,
                      ),
                    ),
                    const SizedBox(width: 20),
                    ElevatedButton(
                      onPressed: () {
                        if (_isConnected) {
                          if (buttonFlashing) {
                            _sendCommand('STOP',0);
                          } else {
                            _sendCommand('RIGHT SIGNAL',0);
                          }
                        }
                      },
                      style: ElevatedButton.styleFrom(
                        backgroundColor: _getButtonColor('RIGHT SIGNAL'),
                        foregroundColor: Colors.black,
                      ),
                      child: const Icon(
                        Icons.arrow_right,
                        color: Color(0xFFB8860B),
                        size: 40,
                      ),
                    ),
                    
                    
                      
                  ],
                ),
                Row(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    ElevatedButton(
                      onPressed: _isConnected ? _toggleMusic : null,
                      style: ElevatedButton.styleFrom(
                        backgroundColor: _musicOn ? Colors.green : Colors.white,
                        foregroundColor: Colors.black,
                      ),
                      child: Icon(
                        _musicOn ? Icons.music_note : Icons.music_off,
                        size: 40,
                        color: _musicOn ? Colors.white : Colors.black,
                      ),
                    ),
                    const SizedBox(width: 20),
                    
                    ElevatedButton(
                      onPressed: _isConnected ? _togglePlatform : null,
                      style: ElevatedButton.styleFrom(
                        backgroundColor: _platformExtended ? Colors.blue : Colors.white,
                        foregroundColor: Colors.black,
                      ),
                      child: Icon(
                        _platformExtended ? Icons.zoom_out_map : Icons.zoom_in_map,
                        size: 40,
                        color: _platformExtended ? Colors.white : Colors.black,
                      ),
                    ),
                ],),
              Row(
                mainAxisAlignment: MainAxisAlignment.center,
                children: [
                  ElevatedButton(
                                         onPressed: () {
                        if (_isConnected) {
                          _sendCommand("INCREASE", 0);
                        }
                      },
                      style: ElevatedButton.styleFrom(
                        backgroundColor: Colors.yellow,
                        foregroundColor: Colors.black,
                      ),
                      
                      child: const Icon(
                        Icons.arrow_upward,
                        color: Color(0xFFB8860B),
                        size: 40,
                      ),
                    ),
                    const SizedBox(width: 20),
                    ElevatedButton(
                                         onPressed: () {
                        if (_isConnected) {
                          _sendCommand("DECREASE", 0);
                        }
                      },
                      style: ElevatedButton.styleFrom(
                        backgroundColor: Colors.yellow,
                        foregroundColor: Colors.black,
                      ),
                      
                      child: const Icon(
                        Icons.arrow_downward,
                        color: Color(0xFFB8860B),
                        size: 40,
                      ),
                    ),
                ],
              )
              ],
            ),
          ),
          // **************** end of command buttons ****************
        ],
      ),
    );
  }
}
