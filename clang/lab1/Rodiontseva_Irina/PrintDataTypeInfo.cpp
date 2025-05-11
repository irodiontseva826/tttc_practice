#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;

namespace {
class PrintDataTypeInfoConsumer : public ASTConsumer {
public:
  void HandleTranslationUnit(ASTContext &Context) override {
    TranslationUnitDecl *TUD = Context.getTranslationUnitDecl();
    
    for (Decl *D : TUD->decls()) {
      if (auto *CRD = dyn_cast<CXXRecordDecl>(D)) {
        if (CRD->isThisDeclarationADefinition() && !CRD->isImplicit()) {
          llvm::outs() << CRD->getNameAsString();
          
          if (CRD->getNumBases() > 0) {
            for (const auto &Base : CRD->bases()) {
              llvm::outs() << " -> " << Base.getType().getAsString();
            }
          }
          
          PrintFields(CRD);
          PrintMethods(CRD);
          llvm::outs() << "\n";
        }
      }
    }
  }

private:
  std::string getAccess(AccessSpecifier AS) {
    switch (AS) {
      case AS_public: return "public";
      case AS_protected: return "protected";
      case AS_private: return "private";
      default: return "none";
    }
  }
  
  void PrintFields(const CXXRecordDecl *CRD) {
    llvm::outs() << "\n|_Fields\n";
    for (const auto *Field : CRD->fields()) {
      llvm::outs() << "| |_ " << Field->getNameAsString() << " (" << Field->getType().getAsString() << "|" << getAccess(Field->getAccess()) << ")\n";
    }
  }
  
  void PrintMethods(const CXXRecordDecl *CRD) {
    llvm::outs() << "|\n|_Methods\n";
    for (const auto *Method : CRD->methods()) {
      if (isa<CXXConstructorDecl>(Method) || isa<CXXDestructorDecl>(Method) || Method->isCopyAssignmentOperator() || Method->isMoveAssignmentOperator()) {
        continue;
      }
      
      llvm::outs() << "| |_ " << Method->getNameAsString() << " (" << Method->getReturnType().getAsString() << "()|" << getAccess(Method->getAccess());
      
      bool isVirtual = Method->isVirtual();
      bool isPureVirtual = Method->isPureVirtual();
      bool isOverride = Method->size_overridden_methods() > 0;

      if (isPureVirtual) {
        llvm::outs() << "|virtual|pure";
      } else if (isOverride) {
        llvm::outs() << "|override";
      } else if (isVirtual) {
        llvm::outs() << "|virtual";
      }
      
      llvm::outs() << ")\n";
    }
  }
};

class PrintDataTypeInfoAction : public PluginASTAction {
protected:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) override {
    return std::make_unique<PrintDataTypeInfoConsumer>();
  }
  
  bool ParseArgs(const CompilerInstance &CI, const std::vector<std::string> &args) override {
    return true;
  }
  
  PluginASTAction::ActionType getActionType() override {
    return AddAfterMainAction;
  }
};
}

static FrontendPluginRegistry::Add<PrintDataTypeInfoAction>
X("print-data-type-info", "Print information about a custom data type");
