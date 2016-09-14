SET (Lua_FOUND FALSE)
SET (LUA_INTERPRETER_TYPE "")
SET (LUA_INTERPRETER_TYPE "LuaJIT")
SET (LUA_LIBRARY_NAME luajit-5.1)
SET (LUA_INCLUDE_DIRS
  /usr/include/luajit-2.0
  /usr/local/include/luajit-2.1
  /usr/local/include/luajit-2.0
  /opt/boxen/homebrew/include/luajit-2.0
  C:/lib/LuaJIT/src
        )


FIND_PATH (LUA_INCLUDE_DIR lua.h ${LUA_INCLUDE_DIRS} NO_DEFAULT_PATH)

FIND_LIBRARY (LUA_LIBRARY NAMES ${LUA_LIBRARY_NAME} lua51 PATHS /usr/lib /usr/local/lib
  /usr/local/lib
  /opt/boxen/homebrew/lib
  C:/lib/LuaJIT/src
  )

FIND_PROGRAM(LUAJIT_BINARY luajit
  /usr/local/bin
  /usr/bin
  C:/lib/LuaJIT/src
  )

MESSAGE(${LUA_INCLUDE_DIR})
IF (LUA_INCLUDE_DIR AND LUA_LIBRARY)
  SET (Lua_FOUND TRUE)
ENDIF (LUA_INCLUDE_DIR AND LUA_LIBRARY)

IF (Lua_FOUND)
  IF (NOT Lua_FIND_QUIETLY)
    MESSAGE(STATUS "Found ${LUA_INTERPRETER_TYPE} library: ${LUA_LIBRARY}")
  ENDIF (NOT Lua_FIND_QUIETLY)
ELSE (Lua_FOUND)
  IF (Lua_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find ${LUA_INTERPRETER_TYPE}")
  ENDIF (Lua_FIND_REQUIRED)
ENDIF (Lua_FOUND)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Lua  DEFAULT_MSG LUA_LIBRARY LUA_INCLUDE_DIR)
