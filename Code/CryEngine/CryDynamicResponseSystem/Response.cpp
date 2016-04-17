// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.

#include "stdafx.h"
#include <CryDynamicResponseSystem/IDynamicResponseCondition.h>
#include <CryString/StringUtils.h>

#include "Response.h"
#include "ResponseInstance.h"
#include "ResponseManager.h"
#include "ResponseSegment.h"
#include "VariableCollection.h"
#include "ResponseSystem.h"
#include "ConditionsCollection.h"
#include <CrySerialization/Enum.h>
#include <CrySerialization/Decorators/BitFlags.h>

using namespace CryDRS;

void CConditionParserHelper::GetResponseVariableValueFromString(const char* szValueString, CVariableValue* pOutValue)
{
	for (int i = 0; szValueString[i] != '\0'; i++)
	{
		if (szValueString[i] == '.' || szValueString[i] == ',')
		{
			//its a float
			*pOutValue = CVariableValue((float)atof(szValueString));
			return;
		}
		else if (szValueString[i] >= 'A' && szValueString[i] <= 'z')
		{
			if (stricmp(szValueString, "true") == 0)
			{
				//its a boolean
				*pOutValue = CVariableValue(true);
			}
			else if (stricmp(szValueString, "false") == 0)
			{
				//its a boolean
				*pOutValue = CVariableValue(false);
			}
			else
			{
				//its a string
				*pOutValue = CVariableValue(CHashedString(szValueString));
			}
			return;
		}
	}

	*pOutValue = CVariableValue(atoi(szValueString));
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
CResponseInstance* CResponse::StartExecution(SSignal& signal)
{
	DRS_DEBUG_DATA_ACTION(AddResponseStarted(signal.m_signalName.GetText()));
	DRS_DEBUG_DATA_ACTION(AddResponseSegmentStarted(&m_baseSegment));

	++m_executionCounter;
	CResponseInstance temp(signal, this);
	if (m_baseSegment.AreConditionsMet(&temp))
	{
		m_lastStartTime = CResponseSystem::GetInstance()->GetCurrentDrsTime();
		CResponseInstance* newResponseInstance = CResponseSystem::GetInstance()->GetResponseManager()->CreateInstance(signal, this);
		newResponseInstance->Execute();
		return newResponseInstance;
	}
	else
	{
		--m_executionCounter;
		DRS_DEBUG_DATA_ACTION(AddResponseInstanceFinished(CResponseSystemDebugDataProvider::eER_NoValidSegment));
	}

	return nullptr;
}

//--------------------------------------------------------------------------------------------------
void CResponse::Serialize(Serialization::IArchive& ar)
{
	ar(m_baseSegment, "BaseSegment", "+<");
}
