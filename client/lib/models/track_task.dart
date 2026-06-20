import 'dart:convert';
import 'dart:typed_data';
import './packet_type.dart';

class TrackTask {
  final String trackName;
  final String artist;
  final String album;
  final String destPlaylistId;
  final String destAuthToken;

  const TrackTask({
    required this.trackName,
    required this.artist,
    required this.album,
    required this.destPlaylistId,
    required this.destAuthToken,
  });

  // helper: writes to buffer [ length | data ]
  static int _writeLengthPrefixedString(
    Uint8List buffer, 
    ByteData byteData, 
    List<int> stringBytes,
    int offset
  ) {
    // 2 byte header to describe length of data
    byteData.setUint16(offset, stringBytes.length, Endian.big);
    offset += 2;
    
    // copy bytes to the buffer
    buffer.setRange(offset, offset + stringBytes.length, stringBytes);
    offset += stringBytes.length;
    
    // return the next write pos
    return offset;
  }

  // serialization for network transfer
  Uint8List toPayloadBytes() {
    final trackNameBytes = utf8.encode(trackName);
    final artistBytes = utf8.encode(artist);
    final albumBytes = utf8.encode(album);
    final destPlaylistIdBytes = utf8.encode(destPlaylistId);
    final destAuthTokenBytes = utf8.encode(destAuthToken);

    // 1 byte for packet header
    // 2 bytes per length of 5 fields
    final packetSize = 1
      + 2 * 5
      + trackNameBytes.length
      + artistBytes.length
      + albumBytes.length
      + destPlaylistIdBytes.length
      + destAuthTokenBytes.length;

    final buffer = Uint8List(packetSize);
    final byteData = ByteData.sublistView(buffer);

    int offset = 0;

    // flag packet as being a track task payload
    buffer[offset++] = PacketType.trackPayload.index;

    // sequentially add the track task strings to the packet
    offset = _writeLengthPrefixedString(buffer, byteData, trackNameBytes, offset);
    offset = _writeLengthPrefixedString(buffer, byteData, artistBytes, offset);
    offset = _writeLengthPrefixedString(buffer, byteData, albumBytes, offset);
    offset = _writeLengthPrefixedString(buffer, byteData, destPlaylistIdBytes, offset);
    offset = _writeLengthPrefixedString(buffer, byteData, destAuthTokenBytes, offset);

    return buffer;
  }
}