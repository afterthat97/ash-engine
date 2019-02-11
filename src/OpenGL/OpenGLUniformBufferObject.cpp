#include <OpenGLUniformBufferObject.h>

QOpenGLFunctions_3_3_Core* OpenGLUniformBufferObject::glFuncs = 0;

OpenGLUniformBufferObject::OpenGLUniformBufferObject() {
    m_id = 0;
    glFuncs = 0;
}

OpenGLUniformBufferObject::~OpenGLUniformBufferObject() {
    if (m_id) destroy();
}

GLuint OpenGLUniformBufferObject::bufferId() const {
    return m_id;
}

bool OpenGLUniformBufferObject::create() {
    if (m_id) destroy();
    if (!glFuncs)
        glFuncs = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    glFuncs->glGenBuffers(1, &m_id);
    return m_id > 0;
}

void OpenGLUniformBufferObject::allocate(int indx, const void * data, int count) {
    if (m_id == 0) return;
    if (!glFuncs) return;
    glFuncs->glBufferData(GL_UNIFORM_BUFFER, count, data, GL_STATIC_DRAW);
    glFuncs->glBindBufferRange(GL_UNIFORM_BUFFER, indx, m_id, 0, count);
}

void OpenGLUniformBufferObject::destroy() {
    if (m_id && glFuncs) {
        glFuncs->glDeleteBuffers(1, &m_id);
        m_id = 0;
    }
}

void OpenGLUniformBufferObject::bind() {
    if (m_id && glFuncs)
        glFuncs->glBindBuffer(GL_UNIFORM_BUFFER, m_id);
}

void OpenGLUniformBufferObject::write(int offset, const void * data, int count) {
    if (m_id && glFuncs)
        glFuncs->glBufferSubData(GL_UNIFORM_BUFFER, offset, count, data);
}

void OpenGLUniformBufferObject::release() {
    if (glFuncs)
        glFuncs->glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OpenGLUniformBufferObject::bindUniformBlock(QOpenGLShaderProgram* shader) {
	if (!glFuncs)
		glFuncs = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    GLuint indx = glFuncs->glGetUniformBlockIndex(shader->programId(), "CameraInfo");
	glFuncs->glUniformBlockBinding(shader->programId(), indx, CAMERA_INFO_BINDING_POINT);
    indx = glFuncs->glGetUniformBlockIndex(shader->programId(), "ModelInfo");
	glFuncs->glUniformBlockBinding(shader->programId(), indx, MODEL_INFO_BINDING_POINT);
    indx = glFuncs->glGetUniformBlockIndex(shader->programId(), "MaterialInfo");
	glFuncs->glUniformBlockBinding(shader->programId(), indx, MATERIAL_INFO_BINDING_POINT);
    indx = glFuncs->glGetUniformBlockIndex(shader->programId(), "LightInfo");
	glFuncs->glUniformBlockBinding(shader->programId(), indx, LIGHT_INFO_BINDING_POINT);
}
