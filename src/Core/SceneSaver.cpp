#include <SceneSaver.h>

SceneSaver::SceneSaver(Scene* scene) {
    m_scene = scene;
}

bool SceneSaver::saveToFile(QString filePath) {
    m_textures.clear();
    for (int i = 0; i < m_scene->models().size(); i++)
        getAllTextures(m_scene->models()[i]);

    QFile file(filePath);
    file.open(QIODevice::WriteOnly);

    if (!file.isOpen()) {
        if (log_level >= LOG_LEVEL_ERROR)
            dout << "Failed to write to file:" << file.errorString();
        m_log += file.errorString();
        return false;
    }

    QDataStream out(&file);

    out << quint32(0xA0B0C0D0); // magic number
    out << quint32(100); // version 1.0.0

    out << m_textures.size();
    for (int i = 0; i < m_textures.size(); i++)
        saveTexture(m_textures[i], out);

    out << 1;
    saveCamera(m_scene->camera(), out);

    out << m_scene->gridlines().size();
    for (int i = 0; i < m_scene->gridlines().size(); i++)
        saveGridline(m_scene->gridlines()[i], out);

    out << m_scene->ambientLights().size();
    for (int i = 0; i < m_scene->ambientLights().size(); i++)
        saveAmbientLight(m_scene->ambientLights()[i], out);

    out << m_scene->directionalLights().size();
    for (int i = 0; i < m_scene->directionalLights().size(); i++)
        saveDirectionalLight(m_scene->directionalLights()[i], out);

    out << m_scene->pointLights().size();
    for (int i = 0; i < m_scene->pointLights().size(); i++)
        savePointLight(m_scene->pointLights()[i], out);

    out << m_scene->spotLights().size();
    for (int i = 0; i < m_scene->spotLights().size(); i++)
        saveSpotLight(m_scene->spotLights()[i], out);

    out << m_scene->models().size();
    for (int i = 0; i < m_scene->models().size(); i++)
        saveModel(m_scene->models()[i], out);

    out << m_scene->m_gridlineNameCounter;
    out << m_scene->m_ambientLightNameCounter;
    out << m_scene->m_directionalLightNameCounter;
    out << m_scene->m_pointLightNameCounter;
    out << m_scene->m_spotLightNameCounter;

    file.close();

    return true;
}

bool SceneSaver::hasErrorLog() {
    return m_log != "";
}

QString SceneSaver::errorLog() {
    QString tmp = m_log;
    m_log = "";
    return tmp;
}

void SceneSaver::getAllTextures(Model * model) {
    for (int i = 0; i < model->childMeshes().size(); i++)
        if (model->childMeshes()[i]->material()) {
            Material * material = model->childMeshes()[i]->material();
            if (material == 0) continue;
            if (!material->diffuseTexture().isNull() && !m_textures.contains(material->diffuseTexture()))
                m_textures.push_back(material->diffuseTexture());
            if (!material->specularTexture().isNull() && !m_textures.contains(material->specularTexture()))
                m_textures.push_back(material->specularTexture());
            if (!material->bumpTexture().isNull() && !m_textures.contains(material->bumpTexture()))
                m_textures.push_back(material->bumpTexture());
        }
    for (int i = 0; i < model->childModels().size(); i++)
        getAllTextures(model->childModels()[i]);
}

void SceneSaver::saveCamera(Camera * camera, QDataStream & out) {
    out << camera->movingSpeed();
    out << camera->fieldOfView();
    out << camera->aspectRatio();
    out << camera->nearPlane();
    out << camera->farPlane();
    out << camera->position();
    out << camera->direction();
}

void SceneSaver::saveGridline(Gridline * gridline, QDataStream & out) {
    out << gridline->objectName();
    out << QVector3D(gridline->xRange().first, gridline->xRange().second, gridline->xStride());
    out << QVector3D(gridline->yRange().first, gridline->yRange().second, gridline->yStride());
    out << QVector3D(gridline->zRange().first, gridline->zRange().second, gridline->zStride());
    out << gridline->color();
}

void SceneSaver::saveAmbientLight(AmbientLight * light, QDataStream & out) {
    out << light->objectName();
    out << light->color();
    out << light->enabled();
    out << light->intensity();
}

void SceneSaver::saveDirectionalLight(DirectionalLight * light, QDataStream & out) {
    out << light->objectName();
    out << light->color();
    out << light->enabled();
    out << light->intensity();
    out << light->direction();
}

void SceneSaver::savePointLight(PointLight * light, QDataStream & out) {
    out << light->objectName();
    out << light->color();
    out << light->enabled();
    out << light->intensity();
    out << light->position();
    out << light->enableAttenuation();
    out << light->attenuationArguments();
}

void SceneSaver::saveSpotLight(SpotLight * light, QDataStream & out) {
    out << light->objectName();
    out << light->color();
    out << light->enabled();
    out << light->intensity();
    out << light->position();
    out << light->direction();
    out << light->innerCutOff();
    out << light->outerCutOff();
    out << light->enableAttenuation();
    out << light->attenuationArguments();
}

void SceneSaver::saveModel(Model * model, QDataStream & out) {
    out << model->objectName();
    out << model->visible();
    out << model->position();
    out << model->rotation();
    out << model->scaling();

    out << model->childMeshes().size();
    for (int i = 0; i < model->childMeshes().size(); i++)
        saveMesh(model->childMeshes()[i], out);

    out << model->childModels().size();
    for (int i = 0; i < model->childModels().size(); i++)
        saveModel(model->childModels()[i], out);
}

void SceneSaver::saveMesh(Mesh * mesh, QDataStream & out) {
    out << mesh->objectName();
    out << mesh->visible();
    out << mesh->meshType();
    out << mesh->position();
    out << mesh->rotation();
    out << mesh->scaling();
    out << mesh->vertices();
    out << mesh->indices();

    out << bool(mesh->material() != 0);
    if (mesh->material())
        saveMaterial(mesh->material(), out);
}

void SceneSaver::saveMaterial(Material * material, QDataStream & out) {
    out << material->objectName();
    out << material->color();
    out << material->ambient();
    out << material->diffuse();
    out << material->specular();
    out << material->shininess();

    int textureNum = 0;
    if (!material->diffuseTexture().isNull()) textureNum++;
    if (!material->specularTexture().isNull()) textureNum++;
    if (!material->bumpTexture().isNull()) textureNum++;
    out << textureNum;

    if (!material->diffuseTexture().isNull()) {
        out << material->diffuseTexture()->textureType();
        out << m_textures.indexOf(material->diffuseTexture());
    }

    if (!material->specularTexture().isNull()) {
        out << material->specularTexture()->textureType();
        out << m_textures.indexOf(material->specularTexture());
    }

    if (!material->bumpTexture().isNull()) {
        out << material->bumpTexture()->textureType();
        out << m_textures.indexOf(material->bumpTexture());
    }
}

void SceneSaver::saveTexture(QSharedPointer<Texture> texture, QDataStream & out) {
    out << texture->objectName();
    out << texture->enabled();
    out << texture->textureType();
    out << texture->image();
}
