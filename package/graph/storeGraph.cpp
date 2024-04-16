
#include <fstream>
#include <tuple>
#include "../utils/types.h"
#include "graph.h"

using namespace std;

int main(int argc, char* argv[]){
    Graph G;

    const string m=argv[1];

    G.LoadFromFile(m); // loading the input file

    G.PrintMetaData();

    return 0;
}