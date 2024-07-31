#include <QCoreApplication>
#include <QCommandLineParser>
#include <QImage>
#include <QtCore>

#include <iostream>
#include "utils/sceneparser.h"
#include "raytracer/raytracer.h"
#include "raytracer/raytracescene.h"
#include <sstream>
#include <iomanip>


/*!
    @param: int number

    @return: string which represents int in correct format

    @brief:
    *Function converts an integer into %d format for FFmpeg
*/
std::string to_format(const int number) {
    std::stringstream ss;
    ss << std::setw(4) << std::setfill('0') << number;
    return ss.str();
}


/*!
    **Main function**

    @brief:
    *Sets up the configuration of the raytracer
    *Calls raytracer to render a scene and saves the image
    *Is capable of saving multiple images that can be animated using FFmpeg
*/
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addPositionalArgument("config", "Path of the config file.");
    parser.process(a);

    ///NOTE: This line writes the stdout into a file raypos.txt located in the root folder
    //freopen("raypos.txt","w",stdout);


    auto positionalArgs = parser.positionalArguments();
    if (positionalArgs.size() != 1) {
        std::cerr << "Not enough arguments. Please provide a path to a config file (.ini) as a command-line argument." << std::endl;
        a.exit(1);
        return 1;
    }


    //int x = -3;




    QSettings settings( positionalArgs[0], QSettings::IniFormat );
    QString iScenePath = settings.value("IO/scene").toString();
    QString oImagePath = settings.value("IO/output").toString();

    RenderData metaData;
    bool success = SceneParser::parse(iScenePath.toStdString(), metaData);

    if (!success) {
        std::cerr << "Error loading scene: \"" << iScenePath.toStdString() << "\"" << std::endl;
        a.exit(1);
        return 1;
    }

    // Raytracing-relevant code

    int width = settings.value("Canvas/width").toInt();
    int height = settings.value("Canvas/height").toInt();

    // Extracting data pointer from Qt's image API
    QImage image = QImage(width, height, QImage::Format_RGBX8888);
    image.fill(Qt::black);
    RGBA *data = reinterpret_cast<RGBA *>(image.bits());

    // Setting up the raytracer
    RayTracer::Config rtConfig{};
    rtConfig.enableShadow        = settings.value("Feature/shadows").toBool();
    rtConfig.enableReflection    = settings.value("Feature/reflect").toBool();
    rtConfig.enableRefraction    = settings.value("Feature/refract").toBool();
    rtConfig.enableTextureMap    = settings.value("Feature/texture").toBool();
    rtConfig.enableTextureFilter = settings.value("Feature/texture-filter").toBool();
    rtConfig.enableParallelism   = settings.value("Feature/parallel").toBool();
    rtConfig.enableSuperSample   = settings.value("Feature/super-sample").toBool();
    rtConfig.enableAcceleration  = settings.value("Feature/acceleration").toBool();
    rtConfig.enableDepthOfField  = settings.value("Feature/depthoffield").toBool();
    rtConfig.maxRecursiveDepth   = settings.value("Settings/maximum-recursive-depth").toInt();
    rtConfig.onlyRenderNormals   = settings.value("Settings/only-render-normals").toBool();
    rtConfig.sample = 100;


//LOPSIDES

    //Animation params
    bool isVideo = false;
    bool focalVariation = true;
    bool appertureVar = false;
    bool animate = false;


    ///FOR ANIMATION WITH CHANGING FOCAL LENGTH
    ///

    if (isVideo){
        for(int i = 0; i <480; i++){
            std::string type;

            if(focalVariation){
                rtConfig.focalLength =    rtConfig.focalLength + 1.f;
                type = "focalChange";
            }

            else if(animate){
                rtConfig.increment = 360;
            }

            else if(appertureVar){
                rtConfig.apperture =    rtConfig.apperture + 0.007f;

                type = "apertureChange";
            }


                RayTracer raytracer{ rtConfig };

            RayTraceScene rtScene{ width, height, metaData };

            // Note that we're passing `data` as a pointer (to its first element)

            raytracer.render(data, rtScene);

            QStringList list =  oImagePath.split(u'.');
            QString element = list.at(0);
            QString elt3 = element.append(type).append(to_format(i)).append(".").append("png");

            success = image.save(elt3);
            if (!success) {
                success = image.save(elt3, "PNG");
            }
            if (success) {
                std::cout << "Saved rendered image to \"" << elt3.toStdString() << "\"" << std::endl;
            } else {
                std::cerr << "Error: failed to save image to \"" << elt3.toStdString() << "\"" << std::endl;
            }
        }
    }




    ///FOR IMAGE GENERATION
    else{
        RayTracer raytracer{ rtConfig };

        RayTraceScene rtScene{ width, height, metaData };

        // Note that we're passing `data` as a pointer (to its first element)
        raytracer.render(data, rtScene);

        QStringList list =  oImagePath.split(u'.');
        QString element = list.at(0);
        QString elt2 = list.at(1);
        //QString elt3 = element.append(to_format(3)).append("SamplesOld").append(QString::number(rtConfig.focalLength)).append(".png");
        QString elt3 = element.append(to_format(3)).append("recract5").append(".png");

        success = image.save(elt3);
        if (!success) {
            success = image.save(elt3, "PNG");
        }
        if (success) {
            std::cout << "Saved rendered image to \"" << elt3.toStdString() << "\"" << std::endl;
        } else {
            std::cerr << "Error: failed to save image to \"" << elt3.toStdString() << "\"" << std::endl;
        }
    }

    ///
    ///



    //DO NOT CHANGE; HANDLES PROGRAM EXIT
    a.exit();
    return 0;
}
