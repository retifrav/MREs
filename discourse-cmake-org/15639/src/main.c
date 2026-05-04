// https://gitlab.com/graphviz/graphviz/-/blob/233597cd428eda284793e363242543b7a0ea5235/dot.demo/demo.c

#include <graphviz/gvc.h>

int main(int argc, char *argv[])
{
    // set up a Graphviz context
    GVC_t *gvc = gvContext();

    // parse CLI arguments: by default it uses argv[0] as the layout engine name,
    // so `graphviz-demo` will fail, and then you either need to launch it with `-K dot`
    // or rename `graphviz-demo` to `dot` (or any other supported layout name)
    gvParseArgs(gvc, argc, argv);

    // create a simple digraph
    Agraph_t *g = agopen("g", Agdirected, 0);
    Agnode_t *n = agnode(g, "n", 1);
    Agnode_t *m = agnode(g, "m", 1);
    (void)agedge(g, n, m, 0, 1);

    // set an attribute, in this case one that affects the visible rendering
    agsafeset(n, "color", "red", "");

    // compute a layout using layout engine from command line args
    gvLayoutJobs(gvc, g);

    // write the graph according to -T and -o options
    gvRenderJobs(gvc, g);

    // free layout data
    gvFreeLayout(gvc, g);

    // free graph structures
    agclose(g);

    // close output file, free context, and return number of errors
    return gvFreeContext(gvc);
}
