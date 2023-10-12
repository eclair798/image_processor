#include "parser.h"

Parser::Parser(int argc, char **argv) {
    program_name = argv[PN_POS];
    input_file_name_ = argv[IF_POS];
    output_file_name_ = argv[OF_POS];
    FilterDescriptor temp;
    for (int i = START_FILTERS_POS; i <= argc; ++i) {
        if (i == argc || *(argv[i]) == '-') {
            if (!(temp.filter_name.empty())) {
                filter_descriptors_.emplace_back(temp);
            }
            temp.Clear();
            if (i != argc) {
                temp.filter_name = argv[i];
            }
        } else {
            temp.filter_params.emplace_back(argv[i]);
        }
    }
}