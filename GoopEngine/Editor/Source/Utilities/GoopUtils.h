/*!*********************************************************************
\file   GoopUtils.h
\author loh.j\@digipen.edu
\date   3-November-2023
\brief
  Contains some helper functions to allow readability for functions
	dealing with filepaths.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <string>

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
	int RandomValue(int min, int max);
}



