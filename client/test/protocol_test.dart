// ignore_for_file: avoid_print

import 'dart:io';
import 'dart:convert';
import 'package:syncopate/models/platform_type.dart';
import 'package:syncopate/models/packet_factory.dart';
import 'package:syncopate/models/track_task.dart';

void main() async {
  final serverTestBin = "../server/build/tests/integration/test_protocol_parsing";

  print("[TEST_CLIENT]: starting server.");
  final serverProc = await Process.start(serverTestBin, []);

  // pipe server output to console
  serverProc.stdout.transform(utf8.decoder).listen((data) {
    print(data.trim());
  });
  serverProc.stderr.transform(utf8.decoder).listen((data) {
    print('[TEST_CLIENT]: $data');
  });

  await Future.delayed(Duration(milliseconds: 1000));

  try {
    print('[TEST_CLIENT]: connecting dart socket to localhost:8080.');
    Socket? socket;
    int retries = 5;

    while (retries > 0) {
      try {
        socket = await Socket.connect('127.0.0.1', 8080, timeout: Duration(milliseconds: 200));
        break;
      } catch (_) {
        retries--;
        if (retries == 0) rethrow;
        await Future.delayed(Duration(milliseconds: 100));
      }
    }

    if (socket == null) {
      print("[TEST_CLIENT]: error, socket failed to connect and is null");
      return;
    }

    final sessionStart = PacketFactory.createSessionStartPacket(
      source: PlatformType.spotify,
      dest: PlatformType.appleMusic,
    );

    final track1 = const TrackTask(
      trackName: 'TRACK_1',
      artist: 'ARTIST_1',
      album: 'ALBUM_1',
      destPlaylistId: 'PLAYLIST',
      destAuthToken: 'AUTH_TOKEN',
    ).toPayloadBytes();

    final track2 = const TrackTask(
      trackName: 'TRACK_2',
      artist: 'ARTIST_2',
      album: 'ALBUM_2',
      destPlaylistId: 'PLAYLIST',
      destAuthToken: 'AUTH_TOKEN',
    ).toPayloadBytes();

    final sessionEnd = PacketFactory.createSessionEndPacket();

    print('[TEST_CLIENT]: Shipping frames over the wire...');
    socket.add(sessionStart);
    await socket.flush();

    socket.add(track1);
    socket.add(track2);
    await socket.flush();

    socket.add(sessionEnd);
    await socket.flush();

    print('[TEST_CLIENT]: packets transmitted, closing socket.');
    await socket.close();

  } catch (e) {
    print('[TEST_CLIENT]: error during socket test: $e');
  } finally {
    print('[TEST_CLIENT]: cleaning up and shutting down background server.');
    serverProc.kill();
  }
}