#include "registerModules.h"
#include <cantProceed.h>
#include <ellLib.h>
#include <freeList.h>
#include <stdio.h>
#include <string.h>
#include <valgrind/valgrind.h>

typedef struct {
  ELLNODE node;
  char *name;
  char *version;
} moduleInfo;

static ELLLIST moduleRegistryList;
static int initRegisterModule = 0;

static int initModuleRegistry() {}

static moduleInfo *allocateModule(char *name, char *version) {
  moduleInfo *module = callocMustSucceed(1, sizeof(moduleInfo),
                                         "Fail to allocate module structure");
  module->name = callocMustSucceed(1, strlen(name) * sizeof(char),
                                   "Fail to allocate module name");
  module->version = callocMustSucceed(1, strlen(version) * sizeof(char),
                                      "Fail to allocate module version");

  strcpy(module->name, name);
  strcpy(module->version, version);
  return module;
}

static inline void initLinkedList(){
  if (!initRegisterModule) {
    ellInit(&moduleRegistryList);
    initRegisterModule = 1;
  }
}

void registerPrintModules() {
  moduleInfo *nextModule;
  initLinkedList();
  printf("Module list:\n");
  if (!ellCount(&moduleRegistryList)) return;
  nextModule = (moduleInfo *)ellFirst((&moduleRegistryList));
  do{
    printf("%s: %s\n", nextModule->name, nextModule->version);
    nextModule = (moduleInfo *)ellFirst(nextModule);
  }while(nextModule != NULL);
}

int registerModule(char *name, char *version) {
  initLinkedList();
  moduleInfo *module = allocateModule(name, version);
  ellAdd(&moduleRegistryList, &(module->node));
  return 0;
}
