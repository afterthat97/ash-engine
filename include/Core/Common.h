#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <climits>
#include <cstdlib>
#include <cstdint>
#include <ctime>
#include <memory>

#include <QApplication>
#include <QByteArray>
#include <QString>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFont>
#include <QTimer>
#include <QMimeData>
#include <QTime>
#include <QImage>
#include <QEvent>
#include <QKeyEvent>

#include <QPoint>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix3x3>
#include <QMatrix4x4>
#include <QQuaternion>

#include <QMessageBox>

#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFramebufferObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QOpenGLWindow>
#include <QOpenGLContext>

#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFunctions_4_0_Core>
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLFunctions_4_2_Core>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLFunctions_4_4_Core>
#include <QOpenGLFunctions_4_5_Core>

#include <QApplication>
#include <QSurfaceFormat>
#include <QMenuBar>
#include <QTreeView>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QMainWindow>
#include <QPushButton>

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QTreeView>
#include <QTreeWidget>

#include <QSplitter>
#include <QMimeData>
#include <QPoint>
#include <QSlider>
#include <QSpinBox>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include <QLineEdit>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QCommonStyle>

#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>
#include <QByteArray>
#include <QDesktopServices>

#include <QImageReader>

#define dout ((qDebug().noquote().nospace() << __FUNCTION__ << "(" << QFileInfo(__FILE__).fileName() << ":" << __LINE__ << "):").quote().space())
#define tab(n) (QByteArray((n) * 4, ' ').data())
#define inf (1000000.0f)
