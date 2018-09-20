char const * const verbs[] = {
    "cry",
    "run",
    "fight",
    "crawl",
    "dance",
};
const size_t number_of_verbs = sizeof(verbs) / sizeof(verbs[0]);

char const * const nouns[] = {
    "slug",
    "tree",
    "mudcrab",
    "mad crab",
    "drunk",
    "tree",
    "rock",
    "god",
};
const size_t number_of_nouns = sizeof(nouns) / sizeof(nouns[0]);

char const *random_verb() {
    return verbs[rand() % number_of_verbs];
}

char const *random_noun() {
    return nouns[rand() % number_of_nouns];
}

void insult1(void) {
    printf("You %s like a %s!\n", random_verb(), random_noun());
}

void insult2(void) {
    printf("I've fought %ss tougher than you!\n", random_noun());
}

void insult3(void) {
    printf("You call that %sing?!\n", random_verb());
}

typedef void (*insult_printer)(void);
insult_printer const insults[] = {
    insult1,
    insult2,
    insult3,
};
const size_t number_of_insults = sizeof(insults) / sizeof(insults[0]);

insult_printer random_insult(void) {
    return insults[rand() % number_of_insults];
}

int main() {
    insult_printer insult = random_insult();
    insult();
    return 0;
}
