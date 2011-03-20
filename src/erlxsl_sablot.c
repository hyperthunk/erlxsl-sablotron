/*
 * erlxsl_sablot.c
 *
 * -----------------------------------------------------------------------------
 * Copyright (c) 2008-2010 Tim Watson (watson.timothy@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * -----------------------------------------------------------------------------
 */

#include <erlxsl.h>
#include <sablot.h>

/*
 * Contains pointers to the various chunks
 * of API data we need for post processing cleanup.
 */
typedef struct async_state {
    SablotSituation situation;
    SablotHandle sHandle;
    SDOM_Document xsl;
    SDOM_Document xml;
} SablotAsyncState;

/* INTERNAL DRIVER FUNCTIONS */
void init_engine(XslEngine*);
static EngineState default_handleTransform(Command*);
static EngineState default_postHandle(Command*);
static void default_shutdown(void*);
static int lookup_command(char*);
static void setup_stub(int, char*);
static EngineState default_command(Command*);

void init_engine(XslEngine *spec) {
    spec->transform = handleTransform;
    spec->after_transform = postHandle;
    spec->shutdown = shutdown;
    spec->command = NULL; //default_command;
};

static EngineState
transform(Command *command) {
    INFO("erlxsl_sablot::transform\n");

    SablotAsyncState* asyncState = command->alloc(sizeof(SablotAsyncState));
    if (asyncState == NULL) {
        return OutOfMemoryError;
    }

    command->async_state = asyncState;

    // NB: these types all represent pointers
    SablotSituation situation;
    SablotHandle sHandle;
    SDOM_Document xsl, xml;

    assert(command != NULL);
    assert(command->result != NULL);

    XslTask* task = get_task(command);

    assert(task != NULL);
    assert(task->input_doc != NULL);
    assert(task->xslt_doc != NULL);

    char *input = get_doc_buffer(task->input_doc);
    assert(input != NULL);

    char *stylesheet = get_doc_buffer(task->xslt_doc);
    assert(stylesheet != NULL);

    if (!SablotCreateSituation(&situation)) {
        return Error;
    }
    asyncState->situation = situation;
    INFO("Situation built ok...");

    if (SablotParseBuffer(situation, input, &xml) != 0) {
        return XmlParseError;
    }
    INFO("Parsed xml data ok...");
    asyncState->xml = xml;

    if (SablotParseStylesheetBuffer(situation, stylesheet, &xsl) != 0) {
        return XslCompileError;
    }
    INFO("Parsed stylesheet ok...");
    asyncState->xsl = xsl;

    INFO("Starting Sablotron API calls...");
    if (SablotCreateProcessorForSituation(situation, &sHandle) != 0) {
        return Error;
    }
    asyncState->sHandle = sHandle;

    SablotAddArgTree(situation, sHandle, "sheet", xsl);
    SablotAddArgTree(situation, sHandle, "data", xml);

    INFO("Processing...");
    if (SablotRunProcessorGen(situation, sHandle,
                              "arg:/sheet", "arg:/data", "arg:/out") != 0) {
        return XslTransformError;
    }

    // TODO: return XslTransformError if this fails....

    SablotGetResultArg(sHandle, "arg:/out", cmd_buff(command));

    // NB: we DO NOT mark the result data as dirty, because it is
    // meant to be freed by sablotron and not by us - not setting
    // the marker means that the erlxsl internal cleanup functions
    // will bypass the payload field
    // command->result->dirty = 1;

    command->result->type = Text;
    return Ok;
};

static EngineState
after_transform(Command *command) {
    INFO("erlxsl_sablot::after_transform\n");
    SablotAsyncState* asyncState = command->async_state;
    if (asyncState != NULL) {
        INFO("Cleanup...");
        SablotSituation situation = asyncState->situation;
        SablotFree(cmd_buff(command));
        SablotDestroyDocument(situation, asyncState->xsl);
        SablotDestroyDocument(situation, asyncState->xml);
        SablotDestroyProcessor(asyncState->sHandle);
        SablotDestroySituation(situation);
        INFO("Finished cleaning...");
        command->release(command->async_state);
        command->async_state = NULL;
    }
    return Ok;
};

static void
shutdown(void *state) {
    INFO("default_shutdown\n");
};


/* Walks the supplied linked list of params and adds them. */
/*
static void addParametersToSituation(SablotSituation situation,
        SablotHandle sHandle, ParameterInfo* parameters) {
    while (NULL != parameters->next) {
        addParametersToSituation(situation, sHandle, parameters->next);
    }
    SablotAddParam(situation, sHandle, parameters->name, parameters->value);
}
*/
