def Settings( **kwargs ):
  return {
    'flags': ['-x', 'c++', '-Wall', '-pedantic', '-Isrc',
    '-Iinclude',
    '-Igodot-cpp/gen/include',
    '-Igodot-cpp/gen/gdextension',
    '-Igodot-cpp/include',
    '-Igodot-cpp/gdextension',
    '-IICon7/bitscpp/include',
    '-IICon7/uSockets/src/libusockets.h',
    '-IICon7/include', '-std=c++17', '-I/usr/include'],
  }
