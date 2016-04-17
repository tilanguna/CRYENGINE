// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.

// -------------------------------------------------------------------------
//  File name:
//  Version:     v1.00
//  Created:     11/02/2015 by Jan Pinter
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef __CCRYDX12VIEW__
	#define __CCRYDX12VIEW__

	#include "CCryDX12Resource.hpp"

	#include "DX12/Resource/Misc/CCryDX12Buffer.hpp"

	#include "DX12/API/DX12View.hpp"

// "view" must be ID3D11*View
// This is potentialy dangerous, but easy & fast...
	#define DX12_EXTRACT_ICRYDX12VIEW(view) \
	  ((view) ? (static_cast<ICryDX12View*>(reinterpret_cast<CCryDX12RenderTargetView*>(view))) : NULL)

	#define DX12_EXTRACT_DX12VIEW(view) \
	  ((view) ? &(static_cast<ICryDX12View*>(reinterpret_cast<CCryDX12RenderTargetView*>(view)))->GetDX12View() : NULL)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct ICryDX12View
{
	virtual ID3D12Resource*  GetD3D12Resource() const = 0;

	virtual NCryDX12::CView& GetDX12View() = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class CCryDX12View : public CCryDX12DeviceChild<T>, public ICryDX12View
{
public:
	DX12_OBJECT(CCryDX12View, CCryDX12DeviceChild<T> );

	virtual ~CCryDX12View()
	{
		m_DX12View.Invalidate();
	}

	ID3D11Resource* GetD3D11Resource() const
	{
		return m_pResource11;
	}

	ID3D12Resource* GetD3D12Resource() const
	{
		return m_rDX12Resource.GetD3D12Resource();
	}

	NCryDX12::CResource& GetDX12Resource() const
	{
		return m_rDX12Resource;
	}

	NCryDX12::CView& GetDX12View()
	{
		return m_DX12View;
	}

	std::string GetResourceName()
	{
		ICryDX12Resource* ires = DX12_EXTRACT_ICRYDX12RESOURCE(m_pResource11.get());
		CCryDX12Resource<ID3D11Resource>* cres = static_cast<CCryDX12Resource<ID3D11Resource>*>(ires);
		return cres ? cres->GetName() : "-";
	}

	void SetBarrier(ID3D12Device* pDevice, ID3D12GraphicsCommandList* pCmdList, UINT newState)
	{
		DX12_EXTRACT_ICRYDX12RESOURCE(m_pResource11.get())->SetBarrier(pDevice, pCmdList, newState);
	}

	#pragma region /* ID3D11View implementation */

	virtual void STDMETHODCALLTYPE GetResource(
	  _Out_ ID3D11Resource** ppResource)
	{
		if (m_pResource11)
		{
			*ppResource = m_pResource11.get();
			(*ppResource)->AddRef();
		}
		else
		{
			*ppResource = NULL;
		}
	}

	#pragma endregion

protected:
	CCryDX12View(ID3D11Resource* pResource11, NCryDX12::EViewType viewType)
		: Super(nullptr, nullptr)
		, m_pResource11(pResource11)
		, m_rDX12Resource(DX12_EXTRACT_ICRYDX12RESOURCE(pResource11)->GetDX12Resource())
	{
		m_DX12View.Init(m_rDX12Resource, viewType);
	}

	NCryDX12::CView m_DX12View;

	DX12_PTR(ID3D11Resource) m_pResource11;

	NCryDX12::CResource& m_rDX12Resource;
};

#endif // __CCRYDX12VIEW__
