#ifndef _PG_DEBUG_HPP_
#define _PG_DEBUG_HPP_

#include <GL/glew.h>

void MessageCallback( GLenum source,
                      GLenum type,
                      GLuint id,
                      GLenum severity,
                      GLsizei length,
                      const GLchar* message,
                      const void* userParam );

void loglDebugOutput(   GLenum source, 
                            GLenum type, 
                            GLuint id, 
                            GLenum severity, 
                            GLsizei length, 
                            const GLchar *message, 
                            const void *userParam);

void loglEnableDebug();

#endif
