#include <string>
#include "pdb_parser.hpp"

BEGIN_JN

class PdbFileParser : public MolParser {
public:
    PdbFileParser(const S &f);
    MolParsedLine *getline();
};

END_JN

