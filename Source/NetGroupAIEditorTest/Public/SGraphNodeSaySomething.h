// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Layout/Visibility.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Input/Reply.h"
#include "KismetNodes/SGraphNodeK2Base.h"


class SVerticalBox;
class UBPNode_SaySomething;

/**
 * 创建自定义的节点widget
 * 动态增加Pin的话，需要在节点上显示一个”加号按钮“，点击之后增加一个“input pin”
 * 基类就是class SGraphNodeK2Base
 * CreateInputSideAddButton()，创建我们需要的添加输入Pin的按钮；
 * OnAddPin()，响应这个按钮的操作
 */
class NETGROUPAIEDITORTEST_API SGraphNodeSaySomething:public SGraphNodeK2Base
{
public:
	

	SLATE_BEGIN_ARGS(SGraphNodeSaySomething) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UBPNode_SaySomething* InNode);
protected:

	//输入节点
	virtual void CreateInputSideAddButton(TSharedPtr<SVerticalBox> InputBox) override;
	
	//响应UI操作
	virtual FReply OnAddPin() override;
	// End of SGraphNode interface


};
