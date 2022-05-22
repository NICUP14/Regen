enum pgen_token_type
{
    literal,
    escape,
    multiplication,
    expansion_end,
    expansion_begin,
    expansion_range,
    expansion_separator,
};

struct pgen_token
{
    size_t len;
    char *symbol;
    enum pgen_token_type type;
};

struct pgen_token PGEN_ESCAPE = {
    .len = 1,
    .symbol = "\\",
    .type = escape,
};

struct pgen_token PGEN_MULTIPLICATION = {
    .len = 1,
    .symbol = "*",
    .type = multiplication,
};

struct pgen_token PGEN_EXPANSION_END = {
    .len = 1,
    .symbol = "}",
    .type = expansion_end,
};

struct pgen_token PGEN_EXPANSION_BEGIN = {
    .len = 1,
    .symbol = "{",
    .type = expansion_begin,
};

struct pgen_token PGEN_EXPANSION_RANGE = {
    .len = 2,
    .symbol = "..",
    .type = expansion_range,
};

struct pgen_token PGEN_EXPANSION_SEPARATOR = {
    .len = 1,
    .symbol = ",",
    .type = expansion_separator,
};

const size_t PGEN_TOKENS_LEN = 6;
struct pgen_token *PGEN_TOKENS[] = {
    &PGEN_ESCAPE,
    &PGEN_MULTIPLICATION,
    &PGEN_EXPANSION_END,
    &PGEN_EXPANSION_BEGIN,
    &PGEN_EXPANSION_RANGE,
    &PGEN_EXPANSION_SEPARATOR,
};
