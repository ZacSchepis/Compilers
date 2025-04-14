/* Name: Zac Schepis
 * Due: Feb/19
 * Probalistic Context Free Grammar
 * All code provided besides the skeleton code template from Jason is my own work.
 */


#include <iostream>
#include <random>

using namespace std;

std::random_device dev;
std::mt19937 rng(dev());
void grammar_Noun(std::string &result);
void grammar_Pronoun(std::string &result);
void grammar_Proper_noun(std::string &result);
void grammar_Verb(std::string &result);
void grammar_Preposition(std::string &result);
void grammar_Nominal(std::string &result);

void grammar_Aux(std::string &result);
void grammar_NP(std::string &result);
void grammar_VP(std::string &result);
void grammar_PP(std::string &result);
void grammar_S(std::string &result);

void grammar_Noun(std::string &result){
    auto dist = std::uniform_real_distribution<double>(0.0, 1.0);
    auto r = dist(rng);
    if(r < .1) {
        result += " ship";
    } else if(r < .4){
        result += " sword";
    } else if(r < .5){
        result += " coin";
    } else if(r < .55){
        result += " booty";
    } else if( r < .62){
        result += " cannon";
    } else if (r < .7) {
        result += " grub";
    } else if(r < .72){
        result += " spyglass";
    } else if(r < .8){
        result += " poopdeck";
    } else if(r < .9){
        result += " banana";
    } else {
        result += " pegleg";
    }
}

void grammar_Pronoun(std::string &result){
    auto dist = std::uniform_real_distribution<double>(0.0, 1.0);
    auto r = dist(rng);
    if(r < .4) {
        result += " I";
    } else if(r < .45){
        result += " she";
    } else if(r < .6){
        result += " me";
    } else {
        result += " you";
    }
}

void grammar_Proper_Noun(std::string &result){
    auto dist = std::uniform_real_distribution<double>(0.0, 1.0);
    auto r = dist(rng);
    if(r < .1) {
        result += " Wano-Kuni";
    } else if( r< .23){
        result += " Sunny";
    } else if(r < .53){
        result += " Luffy";
    } else if( r< .6){
        result += " Sanji";
    } else if(r < .82){
        result += " Neptune";
    } else {
        result += " Brook";
    }
}

void grammar_Verb(std::string &result){
    auto dist = std::uniform_real_distribution<double>(0.0, 1.0);
    auto r = dist(rng);
    if(r < .2) {
        result += " shoot";
    } else if(r < .26){
        result += " swindle";
    } else if(r < .4){
        result += " retreat";
    } else if(r < .48){
        result += " scuttle";
    } else if(r < .51) {
        result += " plunder";
    } else if(r < .6){
        result += " shiver";
    } else if(r < .62){
        result += " hoist";
    } else if(r < .75){
        result += " guzzle";
    } else if(r < .8){
        result += " brawl";
    } else {
        result += " swab";
    }
}
void grammar_Preposition(std::string &result){
    auto dist = std::uniform_real_distribution<double>(0.0, 1.0);
    auto r = dist(rng);
    if(r < .3) {
        result += " beyond";
    } else if(r < .6){
        result += " upon";
    } else if(r < .8){
        result += " under";
    } else if(r < .95){
        result += " into";
    } else {
        result += " through";
    }
}
void grammar_Det(std::string &result){
    auto dist = std::uniform_real_distribution<double>(0.0, 1.0);
    auto r = dist(rng);
    if(r < .1) {
        result += " that";
    } else if(r < .4){
        result += " a";
    } else {
        result += " the";
    }


}
void grammar_Nominal(std::string &result){
    auto dist = std::uniform_real_distribution<double>(0.0, 1.0);
    auto r = dist(rng);
    if(r < .75){
        grammar_Noun(result);
    } else if(r < .95){
        grammar_Nominal(result);
        grammar_Noun(result);
    } else {
        grammar_Nominal(result);
        grammar_PP(result);
    }
}

void grammar_Aux(string &result) {
    auto dist = std::uniform_real_distribution<double>(0.0, 1.0);
    auto r = dist(rng);
    if (r < 0.6)
        result += " does";
    else
        result += " can";
}

void grammar_NP(std::string &result){
    auto dist = std::uniform_real_distribution<double>(0.0, 1.0);
    auto r = dist(rng);
    if(r < .35){
        grammar_Pronoun(result);
    }
    else if(r < .65){
        grammar_Proper_Noun(result);
    }
    else if(r < .85){
        grammar_Det(result);
        grammar_Nominal(result);
    }
    else {
        grammar_Nominal(result);
    }
}

void grammar_VP(std::string &result){
    auto dist = std::uniform_real_distribution<double>(0.0, 1.0);
    auto r = dist(rng);
    if(r < .35){
        grammar_Verb(result);
    } else if(r < .55){
        grammar_Verb(result);
        grammar_NP(result);
    } else if( r < .65){
        grammar_Verb(result);
        grammar_NP(result);
        grammar_PP(result);
    } else if(r < .8){
        grammar_Verb(result);
        grammar_PP(result);
    } else if(r< .85){
        grammar_Verb(result);
        grammar_NP(result);
        grammar_NP(result);
    } else {
        grammar_VP(result);
        grammar_NP(result);
    }
}

void grammar_PP(std::string &result){
    grammar_Preposition(result);
    grammar_NP(result);
}



void grammar_S(std::string &result) {
    auto dist = std::uniform_real_distribution<double>(0.0, 1.0);
    auto r = dist(rng);

    if (r < 0.80) {
        grammar_NP(result);
        grammar_PP(result);
    } else if (r < 0.95) {
        grammar_Aux(result);
        grammar_NP(result);
        grammar_VP(result);
    } else {
        grammar_VP(result);
    }
}

int main() {
    for(int i=0; i<100000; i++){
        std::string r;
        grammar_S(r);
        std::cout << i+1 <<": " << r << std::endl;

    }
    return 0;
}



/* Name: Zac Schepis
 * Due: Feb/19
 * Probalistic Context Free Grammar
 * All code provided besides the skeleton code template from Jason is my own work.
 */