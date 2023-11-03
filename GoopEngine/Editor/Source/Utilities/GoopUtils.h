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
			Takes in a filepath and extracts just the filename of the string.
		\param filePath
			Filepath that you would like to extract the extension of.
		\return
			The filename of the string.
		************************************************************************/
  std::string ExtractFilename(const std::string& filepath);
}