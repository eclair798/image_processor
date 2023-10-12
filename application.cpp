#include "application.h"
#include "pipeline.h"
#include "parser.h"

void Application::Run(int argc, char** argv) {
    Application app(argc, argv);
    Pipeline pipeline(app.parser.filter_descriptors_);
    Image image(app.parser.input_file_name_);
    pipeline.Apply(image);
    image.WriteFile(app.parser.output_file_name_);
}