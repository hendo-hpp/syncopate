import 'dart:typed_data';
import './platform_type.dart';
import './packet_type.dart';

abstract class PacketFactory {
  // send the source and destination platforms in one initial pass
  static Uint8List createSessionStartPacket({
    required PlatformType source,
    required PlatformType dest,
  }) {
    final buffer = Uint8List(3);
    buffer[0] = PacketType.sessionStart.index;
    buffer[1] = source.index;
    buffer[2] = dest.index;
    return buffer;
  }

  // indicate where a packet ends
  static Uint8List createSessionEndPacket() {
    final buffer = Uint8List(1);
    buffer[0] = PacketType.sessionEnd.index;
    return buffer;
  }
}