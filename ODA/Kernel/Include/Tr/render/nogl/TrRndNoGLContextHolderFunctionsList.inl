 GLProcedure0(GLenum, glGetError)
  GLFunction (void,   glEnable, GLenum)
  GLFunction (void,   glDisable, GLenum)
  GLFunction3(void,   glStencilFunc, GLenum, GLint, GLuint)
  GLFunction (void,   glStencilMask, GLuint)
  GLFunction3(void,   glStencilOp, GLenum, GLenum, GLenum)
  GLFunction (void,   glClearStencil, GLint)
  GLFunction (void,   glClear, GLbitfield)
  GLFunction (void,   glDepthFunc, GLenum)
  GLFunction (void,   glDepthMask, GLboolean)
  GLFunction2(void,   glDepthRangef, GLfloat, GLfloat)
  GLFunction (void,   glClearDepthf, GLfloat)
  GLFunction4(void,   glColorMask, GLboolean, GLboolean, GLboolean, GLboolean)
  GLFunction4(void,   glClearColor, GLclampf, GLclampf, GLclampf, GLclampf)
  GLFunction2(void,   glBlendFunc, GLenum, GLenum)
  GLFunction4(void,   glBlendFuncSeparate, GLenum, GLenum, GLenum, GLenum)
  GLFunction (void,   glBlendEquation, GLenum)
  GLFunction2(void,   glBlendEquationSeparate, GLenum, GLenum)
  GLFunction4(void,   glBlendColor, GLfloat, GLfloat, GLfloat, GLfloat)
  GLFunction2(void,   glPolygonOffset, GLfloat, GLfloat)
  GLFunction (void,   glCullFace, GLenum)
  GLFunction4(void,   glScissor, GLint, GLint, GLsizei, GLsizei)
                      
  GLFunction (void,   glActiveTexture, GLenum)
  GLFunction3(void,   glTexParameteri, GLenum, GLenum, GLint)
  GLFunction2(void,   glGenTextures, GLsizei, GLuint*)
  GLFunction2(void,   glBindTexture, GLenum, GLuint)
  GLFunction9(void,   glTexImage2D, GLenum, GLint, GLint, GLsizei, GLsizei, 
                                    GLint, GLenum, GLenum, const GLvoid*)
  GLFunction2(void,   glDeleteTextures, GLsizei, const GLuint*)
  GLFunction2(void,   glPixelStorei, GLenum, GLint)
  GLFunction7(void,   glReadPixels, GLint, GLint, GLsizei, GLsizei, GLenum, 
                                    GLenum, GLvoid*)
  GLFunction2(void,   glGenFramebuffers, GLsizei, GLuint*)
  GLFunction2(void,   glBindFramebuffer, GLenum, GLuint)
  GLFunction5(void,   glFramebufferTexture2D, GLenum, GLenum, GLenum, GLuint, 
                                              GLint);
  GLFunction4(void,   glFramebufferRenderbuffer, GLenum, GLenum, GLenum, GLuint)
  GLFunction2(void,   glDeleteRenderbuffers, GLsizei, const GLuint*)
  GLFunction2(void,   glGenRenderbuffers, GLsizei, GLuint*)
  GLFunction2(void,   glBindRenderbuffer, GLenum, GLuint)
  GLFunction4(void,   glRenderbufferStorage, GLenum, GLenum, GLsizei, GLsizei)
  GLProcedure(GLenum, glCheckFramebufferStatus, GLenum)
  GLFunction2(void,   glDeleteFramebuffers, GLsizei, const GLuint*)
      //need metal realization or path-throw:
  GLFunction4(void,   glViewport, GLint, GLint, GLsizei, GLsizei)
  GLFunction2(void,   glGetIntegerv, GLenum, GLint*)
  GLFunction3(void,   glDrawArrays, GLenum, GLint, GLsizei)
  GLFunction2(void,   glBindBuffer, GLenum, GLuint)
  GLFunction2(void,   glGenBuffers, GLsizei, GLuint*)  
  GLFunction4(void,   glBufferData, GLenum, GLsizeiptr, const void *, GLenum);
  GLFunction4(void,   glBufferSubData, GLenum, GLintptr, GLsizeiptr, const void *);
  GLFunction2(void,   glDeleteBuffers, GLsizei, const GLuint *);
  GLFunction6(void,   glVertexAttribPointer, GLuint, GLint, GLenum, GLboolean, GLsizei, const void *)
  GLFunction (void,   glEnableVertexAttribArray, GLuint)
  GLFunction (void,   glDisableVertexAttribArray, GLuint)
typedef const GLubyte* GLCstr;
  GLProcedure(GLCstr, glGetString, GLenum);
  GLFunction2(void,   glGetFloatv, GLenum , GLfloat *);
  GLFunction (void,   glLineWidth, GLfloat);
  GLFunction2(void,   glHint, GLenum, GLenum);
  GLFunction3(void,   glTexParameterfv, GLenum, GLenum, const GLfloat *);
  GLFunction4(void,   glDrawElements, GLenum, GLsizei, GLenum, const void *);
  GLFunction2(void,   glUniform1f, GLint, GLfloat);
  GLFunction3(void,   glUniform1fv, GLint, GLsizei, const GLfloat *);
  GLFunction3(void,   glUniform2f, GLint, GLfloat, GLfloat);
  GLFunction3(void,   glUniform2fv, GLint, GLsizei, const GLfloat *);
  GLFunction4(void,   glUniform3f, GLint, GLfloat, GLfloat, GLfloat);
  GLFunction3(void,   glUniform3fv, GLint, GLsizei, const GLfloat *);
  GLFunction5(void,   glUniform4f, GLint, GLfloat, GLfloat, GLfloat, GLfloat);
  GLFunction3(void,   glUniform4fv, GLint, GLsizei, const GLfloat *);
  GLFunction2(void,   glUniform1i, GLint, GLint);
  GLFunction3(void,   glUniform1iv, GLint, GLsizei, const GLint *);
  GLFunction3(void,   glUniform4iv, GLint, GLsizei, const GLint *);
  GLFunction4(void,   glUniformMatrix3fv, GLint, GLsizei, GLboolean, const GLfloat *);
  GLFunction4(void,   glUniformMatrix4fv, GLint, GLsizei, GLboolean, const GLfloat *);
  GLFunction (void,   glUseProgram, GLuint)

#undef GLProcedure0
#undef GLProcedure
#undef GLFunction
#undef GLFunction2
#undef GLFunction3
#undef GLFunction4
#undef GLFunction5
#undef GLFunction6
#undef GLFunction7
#undef GLFunction9