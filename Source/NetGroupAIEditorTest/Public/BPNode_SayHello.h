// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "KismetCompiler.h"	// KismetCompiler
#include "EdGraph/EdGraph.h"	// Engine
#include "EdGraph/EdGraphPin.h"	// Engine
#include "BPNode_SayHello.generated.h"

/**
 * 
 */
UCLASS()
class NETGROUPAIEDITORTEST_API UBPNode_SayHello : public UK2Node
{
	GENERATED_BODY()
public:

	//add input&output pin
	virtual void AllocateDefaultPins()override;
		
	// UEdGraphNode interface
	virtual FText GetTooltipText() const override { return FText::FromString(TEXT("a hello world node")); }
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override { return FText::FromString(TEXT("Say Hello World")); }
	// End of UEdGraphNode interface

	// UK2Node interface
	virtual FText GetMenuCategory() const { return FText::FromString(TEXT("MyBlueprintNodes")); }
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	
	//蓝图节点编译函数-》通过把这个节点展开成一个已有节点类型组成的Sub Graph来实现本节点的功能
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	// End of UK2Node interface

	//output pin
	UEdGraphPin* GetThenPin() const;
	
};
