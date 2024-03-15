/*!*********************************************************************
\file   GoopUtils.h
\author loh.j\@digipen.edu
\date   3-November-2023
\brief
  Contains some helper functions to allow readability for functions
	dealing with filepaths.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <string>
#include <random>

namespace GE::GoopUtils
{
	/*!*********************************************************************
	\brief
		Takes in a string and splits it into a vector of strings delimtited
		by whitespace.
	\param input
		String that you would like to split.
	\return
		Vector of strings.
	************************************************************************/
	std::vector<std::string> SplitStringByPipe(const std::string& input);

  /*!*********************************************************************
  \brief
    Reloads the asset manager's and the Object Factory's maps.
  ************************************************************************/
  void ReloadFileData();

	/*!*********************************************************************
	\brief
		Takes in a filepath and extracts the extension of the string.
	\param filePath
		Filepath that you would like to extract the extension of.
	\return
		The extension of the filepath.
	************************************************************************/
  std::string GetFileExtension(const std::string& filePath);

	/*!*********************************************************************
	\brief
		Takes in a filepath and extracts the previous folder and the filename
	\param filePath
		Filepath that you would like to extract.
	\return
		The previous folder and the filename of the filepath.
	************************************************************************/
	std::string ExtractPrevFolderAndFileName(const std::string& filepath);

	/*!*********************************************************************
	\brief
		Takes in a filepath and extracts just the filename of the string.
	\param filePath
		Filepath that you would like to extract the extension of.
	\return
		The filename of the string.
	************************************************************************/
  std::string ExtractFilename(const std::string& filepath);

	/*!*********************************************************************
	\brief 
	  Creates a random int value between a range [min, max]

	\param min
		Min value of the range

	\param max
		Max value of the range

	\return
		Random int value
	************************************************************************/
	template <typename T>
	T RandomValue(T min, T max)
	{
		// 1 time initialize
		static std::random_device rd;
		static std::mt19937_64 gen(rd());

		// Create distrubition based on range
		std::uniform_real_distribution dist(0.0f, 1.0f);
		return static_cast<T>(min + (max - min) * dist(gen));
	}

	/*!*********************************************************************
	\brief
	  Returns a value interpolated between 2 values.

	\param start
		Start of the range

	\param end
		End of the range

	\param interval
		Interval in the range to find the value
	  
	\return
	  Value between start and end at interval percentage (0-1)
	************************************************************************/
	template <typename T>
	T Lerp(T start, T end, float interval)
	{
		return start + (end - start) * interval;
	}

	/*!*********************************************************************
	\brief
	  Returns the interval (0-1) value of a given value between start and end.

	\param value
		Value too find

	\param start
		Start of range

	\param end
	  End of range

	\return
	  Value between (0-1) of the given range where the input value sits 
	************************************************************************/
	template <typename T>
	float InverseLerp(T value, T start, T end)
	{
		return static_cast<float>((value - start) / (end - start));
	}
}



