/*
 Copyright 2010-2013 Tarik Sekmen.

 All Rights Reserved.

 Written by Tarik Sekmen <tarik@ilixi.org>.

 This file is part of ilixi.

 ilixi is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 ilixi is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with ilixi.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <graphics/FontPack.h>
#include <lib/FileSystem.h>
#include <lib/XMLReader.h>
#include <core/Logger.h>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <Debug.h>

namespace ilixi
{

D_DEBUG_DOMAIN(ILX_FONTPACK, "ilixi/graphics/FontPack", "FontPack");

FontPack::FontPack()
        : _buttonFont(NULL),
          _defaultFont(NULL),
          _inputFont(NULL),
          _titleFont(NULL),
          _infoFont(NULL)
{
}

FontPack::~FontPack()
{
    release();
}

Font*
FontPack::getFont(StyleHint::FontHint font) const
{
    switch (font)
    {
    case StyleHint::ButtonFont:
        return _buttonFont;
    case StyleHint::TitleFont:
        return _titleFont;
    case StyleHint::InputFont:
        return _inputFont;
    case StyleHint::InfoFont:
        return _infoFont;
    default:
        return _defaultFont;
    }
}

bool
FontPack::parseFonts(const char* fontsFile)
{
    ILOG_TRACE(ILX_FONTPACK);
    ILOG_DEBUG(ILX_FONTPACK, " -> file: %s\n", fontsFile);
    DBG("Font file: %s \n", fontsFile);

    std::string cacheFile = PrintF("%s%u.sxml", FileSystem::ilxDirectory().c_str(), createHash(fontsFile));

//    printf("ilxDirectory: %s, cacheFile: %s \n", FileSystem::ilxDirectory().c_str(), cacheFile.c_str());
    ILOG_DEBUG(ILX_FONTPACK, " -> cache file: %s\n", cacheFile.c_str());
    if (difftime(FileSystem::getModificationTime(cacheFile), FileSystem::getModificationTime(fontsFile)) > 0)
    {
//        printf("%s::%s<%d> \n", __FILE__, __FUNCTION__, __LINE__);
        ILOG_DEBUG(ILX_FONTPACK, " -> Parsing cached fonts file.\n");
        std::ifstream ifs(cacheFile.c_str(), std::ios::in);
        ifs >> *this;
        ifs.close();
        ILOG_INFO(ILX_FONTPACK, "Parsed cached fonts file: %s\n", cacheFile.c_str());
//        printf("%s::%s<%d> \n", __FILE__, __FUNCTION__, __LINE__);
    } else
    {
//        printf("%s::%s<%d> \n", __FILE__, __FUNCTION__, __LINE__);
        XMLReader xml;
        if (xml.loadFile(fontsFile) == false)
        {
            ILOG_FATAL(ILX_FONTPACK, "Could not parse fonts!\n");
            return false;
        }
//        printf("%s::%s<%d> \n", __FILE__, __FUNCTION__, __LINE__);

        xmlNodePtr node = xml.currentNode();

        release();

//        printf("%s::%s<%d> \n", __FILE__, __FUNCTION__, __LINE__);
        while (node != NULL)
        {
//            printf("%s::%s<%d> font: %s \n", __FILE__, __FUNCTION__, __LINE__, node->name);
            ILOG_DEBUG(ILX_FONTPACK, " -> font: %s...\n", node->name);
            xmlChar* fileC = xmlNodeGetContent(node->children);
            std::string namefileC((char*)fileC);
//            printf("%s::%s<%d> fileC: %s\n", __FILE__, __FUNCTION__, __LINE__, namefileC.c_str());
            xmlChar* sizeC = xmlNodeGetContent(node->children->next);
//            printf("%s::%s<%d> sizeC: %s \n", __FILE__, __FUNCTION__, __LINE__, (char*)sizeC);
            xmlChar* styleC = xmlNodeGetContent(node->children->next->next);
//            printf("%s::%s<%d> styleC: %s \n", __FILE__, __FUNCTION__, __LINE__, (char*)styleC);

            Font::Style fontStyle = Font::Plain;
            if (styleC)
            {
//                printf("%s::%s<%d> \n", __FILE__, __FUNCTION__, __LINE__);
                if (xmlStrcmp(styleC, (xmlChar *) "italic") == 0) {
                    fontStyle = Font::Italic;
//                    printf("%s::%s<%d> \n", __FILE__, __FUNCTION__, __LINE__);
                } else if (xmlStrcmp(styleC, (xmlChar *) "bold") == 0){
                    fontStyle = Font::Bold;
//                    printf("%s::%s<%d> \n", __FILE__, __FUNCTION__, __LINE__);
                }
                xmlFree(styleC);
//                printf("%s::%s<%d> \n", __FILE__, __FUNCTION__, __LINE__);
            }

//            printf("%s::%s<%d> \n", __FILE__, __FUNCTION__, __LINE__);
            if (xmlStrcmp(node->name, (xmlChar*) "DefaultFont") == 0)
            {
//                printf("%s::%s<%d> \n", __FILE__, __FUNCTION__, __LINE__);
                _defaultFont = new Font(namefileC, atoi((char*) sizeC));
//                printf("%s::%s<%d> \n", __FILE__, __FUNCTION__, __LINE__);
                _defaultFont->setStyle(fontStyle);
//                printf("%s::%s<%d> \n", __FILE__, __FUNCTION__, __LINE__);
                _defaultFont->dfbFont();
//                printf("%s::%s<%d> \n", __FILE__, __FUNCTION__, __LINE__);
            } else if (xmlStrcmp(node->name, (xmlChar*) "ButtonFont") == 0)
            {
//                printf("%s::%s<%d> \n", __FILE__, __FUNCTION__, __LINE__);
                _buttonFont = new Font(namefileC, atoi((char*) sizeC));
//                printf("%s::%s<%d> \n", __FILE__, __FUNCTION__, __LINE__);
                _buttonFont->setStyle(fontStyle);
//                printf("%s::%s<%d> \n", __FILE__, __FUNCTION__, __LINE__);
                _buttonFont->dfbFont();
//                printf("%s::%s<%d> \n", __FILE__, __FUNCTION__, __LINE__);
            }

            else if (xmlStrcmp(node->name, (xmlChar*) "InputFont") == 0)
            {
                _inputFont = new Font(namefileC, atoi((char*) sizeC));
                _inputFont->setStyle(fontStyle);
                _inputFont->dfbFont();
            }

            else if (xmlStrcmp(node->name, (xmlChar*) "TitleFont") == 0)
            {
                _titleFont = new Font(namefileC, atoi((char*) sizeC));
                _titleFont->setStyle(fontStyle);
                _titleFont->dfbFont();
            }

            else if (xmlStrcmp(node->name, (xmlChar*) "InfoFont") == 0)
            {
                _infoFont = new Font(namefileC, atoi((char*) sizeC));
                _infoFont->setStyle(fontStyle);
                _infoFont->dfbFont();
            }
//            printf("%s::%s<%d> \n", __FILE__, __FUNCTION__, __LINE__);

            xmlFree(fileC);
            xmlFree(sizeC);
//            printf("%s::%s<%d> \n", __FILE__, __FUNCTION__, __LINE__);
            node = node->next;
        }
        ILOG_INFO(ILX_FONTPACK, "Parsed fonts file: %s\n", fontsFile);

//        printf("%s::%s<%d> \n", __FILE__, __FUNCTION__, __LINE__);

        std::ofstream ofs(cacheFile.c_str(), std::ios::out);
        ofs.seekp(0, std::ios::beg);
        ofs << *this;
        ofs.close();
        ILOG_INFO(ILX_FONTPACK, "Created cached fonts file: %s\n", cacheFile.c_str());
//        printf("%s::%s<%d> \n", __FILE__, __FUNCTION__, __LINE__);
    }

    return true;
}

void
FontPack::release()
{
    delete _buttonFont;
    delete _defaultFont;
    delete _inputFont;
    delete _titleFont;
    delete _infoFont;

    _buttonFont = NULL;
    _defaultFont = NULL;
    _inputFont = NULL;
    _titleFont = NULL;
    _infoFont = NULL;
}

std::istream&
operator>>(std::istream& is, FontPack& obj)
{
    obj.release();

    obj._buttonFont = new Font();
    obj._defaultFont = new Font();
    obj._inputFont = new Font();
    obj._titleFont = new Font();
    obj._infoFont = new Font();

    is >> *obj._buttonFont;
    is.ignore(1);
    is >> *obj._defaultFont;
    is.ignore(1);
    is >> *obj._inputFont;
    is.ignore(1);
    is >> *obj._titleFont;
    is.ignore(1);
    is >> *obj._infoFont;
    is.ignore(1);

    *obj._buttonFont->dfbFont();
    *obj._defaultFont->dfbFont();
    *obj._inputFont->dfbFont();
    *obj._titleFont->dfbFont();
    *obj._infoFont->dfbFont();

    return is;
}

std::ostream&
operator<<(std::ostream& os, const FontPack& obj)
{
    os << *obj._buttonFont << std::endl;
    os << *obj._defaultFont << std::endl;
    os << *obj._inputFont << std::endl;
    os << *obj._titleFont << std::endl;
    os << *obj._infoFont << std::endl;

    return os;
}

} /* namespace ilixi */
