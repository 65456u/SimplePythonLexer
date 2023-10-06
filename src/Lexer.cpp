
#include "../include/Lexer.h"
void Lexer::printSummary(std::ostream &ostream) {
    ostream << std::endl;
    ostream << "********Summary********" << std::endl;
    ostream << "Total Characters:" << this->characterCount << std::endl;
    ostream << "Total Delimiters:" << this->delimiterCount << std::endl;
    ostream << "Total Operators:" << this->operatorCount << std::endl;
    ostream << std::endl;
    ostream << "Literal Table" << std::endl;
    printLiteralTable(ostream, this->literalTable);
    ostream << std::endl;
    ostream << "Identifier Table" << std::endl;
    this->identifierTable.print();
}
void Lexer::run(std::ostream &ostream) {
    int c;
    while ((c = getNextChar()) != EOF) {
        try {
            if (isdigit(c)) {
                digitHandler(c, ostream);
            } else if (isWhiteSpace(c)) {
                continue;
            } else if (c == '\n') {
                this->currentRow ++;
                this->currentCol = 1;
            } else if (isalpha(c) or c == '_') {
                alphaHandler(c, ostream);
            } else if (isDelimiterChar(c)) {
                punctuationHandler(c, ostream);
            } else if (c == '\'' or c == '\"') {
                stringHandler(c, ostream);
            } else if (isUnusedChar(c)) {
                throw LexerException(pLexer::CHAR_ERROR, "Unused Character");
            } else if (c == '#') {
                commentHandler(c, ostream);
            }
        } catch (LexerException &exception) {
            exception.print(std::cerr, this->currentRow, this->currentCol);
            int next;
            std::istream &is = this->istream;
            while ((next = is.peek()) != EOF) {
                if (isDelimiterChar(next) or isWhiteSpace(next)) {
                    break;
                } else {
                    getNextChar();
                }
            }
        }
    }
    printSummary(ostream);
}
int Lexer::getNextChar() {
    this->currentCol ++;
    this->characterCount ++;
    return this->istream.get();
}


