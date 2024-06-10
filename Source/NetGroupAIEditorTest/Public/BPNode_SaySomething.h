// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "KismetCompiler.h"	// KismetCompiler
#include "EdGraph/EdGraph.h"	// Engine
#include "EdGraph/EdGraphPin.h"	// Engine
#include "EdGraph/EdGraphSchema.h"
#include "BPNode_SaySomething.generated.h"

/**
 * 
 */
UCLASS()
class NETGROUPAIEDITORTEST_API UBPNode_SaySomething : public UK2Node
{
	GENERATED_BODY()

public:

	//分配默认引脚
	virtual void AllocateDefaultPins()override;

	
	// UEdGraphNode interface
	
	virtual FText GetTooltipText() const override { return FText::FromString(TEXT("a dynamic pin node")); }
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override { return FText::FromString(TEXT("Say Something")); }
	
	//操作MenuBuilder,添加菜单项并绑定到UIAction到成员函数UBONode_SaySomething::RemoveInputPin
//	virtual void GetContextMenuActions(const FGraphNodeContextMenuBuilder& Context) const override;

	//右键菜单
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context)const override;
	// End of UEdGraphNode interface

	// UK2Node interface
	virtual FText GetMenuCategory() const { return FText::FromString(TEXT("MyBlueprintNodes")); }
	//将节点添加到蓝图视图
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;



	//蓝图节点编译函数-》通过把这个节点展开成一个已有节点类型组成的Sub Graph来实现本节点的功能
	//创建一个UK2Node_CallFunction的实例，然后把自身节点的两端的Exec pin重定向到这个node的两端
	//使用函数参数名称，找到UK2Node_CallFunction节点的输入pin,把他连接到一个新建的UK2Node_MakeArray的节点实例上
	//把自己所有的输入变量pin，重定向到UK2Node_MakeArray的输入上(需要为他动态添加新的pin)
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	// End of UK2Node interface

	//output pin
	UEdGraphPin* GetThenPin() const;

	
	// Begin of dynamic pin
	// 创建一个可视小部件在图形编辑器的图形面板中表示这个节点
	//创建UBONode_SaySomething对象的时候，使用SGraphNodeSaySomething这个widget
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;




	//添加pin的核心操作->通过SGraphNOdeSaySomething 里面的OnAddPinIn UI操作来调用此函数
	void AddPinToNode();

	
	void RemoveInputPin(UEdGraphPin* Pin);
	// End

private:
	UPROPERTY()
		TArray<FName> ArgPinNames;


	
};
