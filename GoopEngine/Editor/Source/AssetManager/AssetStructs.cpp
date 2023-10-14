/*!*********************************************************************
\file   AssetStructs.cpp
\author loh.j@digipen.edu
\date   28 September 2023
\brief
  Contains classes required for AssetManager.cpp.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <pch.h>
#include <AssetManager/AssetStructs.h>

namespace GE::AssetManager
{
	IDGenerator::~IDGenerator()
	{
		m_recycledID.clear();
	}

	int IDGenerator::GenerateID()
	{
		if (!m_recycledID.empty())
		{
			int recycledID = *m_recycledID.begin();
			m_recycledID.erase(recycledID);
			return recycledID;
		}
		return m_nextID++;
	}
	void IDGenerator::FreeID(int id)
	{
		m_recycledID.insert(id);
	}

	ImageData ImageData::Null()
	{
		return ImageData();
	}

	void ImageData::SetName(const std::string& name)
	{
		this->m_name = name;
	}

	void ImageData::SetInfo(int width, int height, int channels, unsigned char* data)
	{
		this->m_width = width;
		this->m_height = height;
		this->m_channels = channels;
		this->m_data = data;
	}

	unsigned char* ImageData::GetData()const
	{
		return this->m_data;
	}

	unsigned int ImageData::GetWidth()const
	{
		return this->m_width;
	}

	unsigned int ImageData::GetHeight()const
	{
		return this->m_height;
	}

	unsigned int ImageData::GetChannels()const
	{
		return this->m_channels;
	}

	std::string const& ImageData::GetName()const
	{
		return this->m_name;
	}

	int ImageData::GetID()const
	{
		return this->m_id;
	}
}