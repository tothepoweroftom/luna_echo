/* ------------------------------------------------------------
name: "Macalla "
Code generated with Faust 2.79.3 (https://faust.grame.fr)
Compilation options: -a faustMinimal.h -lang cpp -i -ct 1 -cn Macalla -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 32
------------------------------------------------------------ */

#ifndef  __Macalla_H__
#define  __Macalla_H__

#include <cmath>
#include <cstring>

/************************** BEGIN MapUI.h ******************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ***********************************************************************/

#ifndef FAUST_MAPUI_H
#define FAUST_MAPUI_H

#include <vector>
#include <map>
#include <string>
#include <stdio.h>

/************************** BEGIN UI.h *****************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ********************************************************************/

#ifndef __UI_H__
#define __UI_H__

/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ***************************************************************************/

#ifndef __export__
#define __export__

// Version as a global string
#define FAUSTVERSION "2.79.3"

// Version as separated [major,minor,patch] values
#define FAUSTMAJORVERSION 2
#define FAUSTMINORVERSION 79
#define FAUSTPATCHVERSION 3

// Use FAUST_API for code that is part of the external API but is also compiled in faust and libfaust
// Use LIBFAUST_API for code that is compiled in faust and libfaust

#ifdef _WIN32
    #pragma warning (disable: 4251)
    #ifdef FAUST_EXE
        #define FAUST_API
        #define LIBFAUST_API
    #elif FAUST_LIB
        #define FAUST_API __declspec(dllexport)
        #define LIBFAUST_API __declspec(dllexport)
    #else
        #define FAUST_API
        #define LIBFAUST_API 
    #endif
#else
    #ifdef FAUST_EXE
        #define FAUST_API
        #define LIBFAUST_API
    #else
        #define FAUST_API __attribute__((visibility("default")))
        #define LIBFAUST_API __attribute__((visibility("default")))
    #endif
#endif

#endif

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

/*******************************************************************************
 * UI : Faust DSP User Interface
 * User Interface as expected by the buildUserInterface() method of a DSP.
 * This abstract class contains only the method that the Faust compiler can
 * generate to describe a DSP user interface.
 ******************************************************************************/

struct Soundfile;

template <typename REAL>
struct FAUST_API UIReal {
    
    UIReal() {}
    virtual ~UIReal() {}
    
    // -- widget's layouts
    
    virtual void openTabBox(const char* label) = 0;
    virtual void openHorizontalBox(const char* label) = 0;
    virtual void openVerticalBox(const char* label) = 0;
    virtual void closeBox() = 0;
    
    // -- active widgets
    
    virtual void addButton(const char* label, REAL* zone) = 0;
    virtual void addCheckButton(const char* label, REAL* zone) = 0;
    virtual void addVerticalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addHorizontalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addNumEntry(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    
    // -- passive widgets
    
    virtual void addHorizontalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    virtual void addVerticalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    
    // -- soundfiles
    
    virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;
    
    // -- metadata declarations
    
    virtual void declare(REAL* /*zone*/, const char* /*key*/, const char* /*val*/) {}

    // To be used by LLVM client
    virtual int sizeOfFAUSTFLOAT() { return sizeof(FAUSTFLOAT); }
};

struct FAUST_API UI : public UIReal<FAUSTFLOAT> {
    UI() {}
    virtual ~UI() {}
#ifdef DAISY_NO_RTTI
    virtual bool isSoundUI() const { return false; }
    virtual bool isMidiInterface() const { return false; }
#endif
};

#endif
/**************************  END  UI.h **************************/
/************************** BEGIN PathBuilder.h **************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __PathBuilder__
#define __PathBuilder__

#include <vector>
#include <set>
#include <map>
#include <string>
#include <algorithm>


/*******************************************************************************
 * PathBuilder : Faust User Interface
 * Helper class to build complete hierarchical path for UI items.
 ******************************************************************************/

class FAUST_API PathBuilder {

    protected:
    
        std::vector<std::string> fControlsLevel;
        std::vector<std::string> fFullPaths;
        std::map<std::string, std::string> fFull2Short;  // filled by computeShortNames()
    
        /**
         * @brief check if a character is acceptable for an ID
         *
         * @param c
         * @return true is the character is acceptable for an ID
         */
        bool isIDChar(char c) const
        {
            return ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) || ((c >= '0') && (c <= '9'));
        }
    
        /**
         * @brief remove all "/0x00" parts
         *
         * @param src
         * @return modified string
         */
        std::string remove0x00(const std::string& src_aux) const
        {
            std::string src = src_aux;
            std::string from = "/0x00";
            std::string to = "";
            size_t pos = std::string::npos;
            while ((pos = src.find(from)) && (pos != std::string::npos)) {
                src = src.replace(pos, from.length(), to);
            }
            return src;
        }
    
        /**
         * @brief replace all non ID char with '_' (one '_' may replace several non ID char)
         *
         * @param src
         * @return modified string
         */
        std::string str2ID(const std::string& src) const
        {
            std::string dst;
            bool need_underscore = false;
            for (char c : src) {
                if (isIDChar(c) || (c == '/')) {
                    if (need_underscore) {
                        dst.push_back('_');
                        need_underscore = false;
                    }
                    dst.push_back(c);
                } else {
                    need_underscore = true;
                }
            }
            return dst;
        }
    
        /**
         * @brief Keep only the last n slash-parts
         *
         * @param src
         * @param n : 1 indicates the last slash-part
         * @return modified string
         */
        std::string cut(const std::string& src, int n) const
        {
            std::string rdst;
            for (int i = int(src.length())-1; i >= 0; i--) {
                char c = src[i];
                if (c != '/') {
                    rdst.push_back(c);
                } else if (n == 1) {
                    std::string dst;
                    for (int j = int(rdst.length())-1; j >= 0; j--) {
                        dst.push_back(rdst[j]);
                    }
                    return dst;
                } else {
                    n--;
                    rdst.push_back(c);
                }
            }
            return src;
        }
    
        void addFullPath(const std::string& label) { fFullPaths.push_back(buildPath(label)); }
    
        /**
         * @brief Compute the mapping between full path and short names
         */
        void computeShortNames()
        {
            std::vector<std::string>           uniquePaths;  // all full paths transformed but made unique with a prefix
            std::map<std::string, std::string> unique2full;  // all full paths transformed but made unique with a prefix
            char num_buffer[16];
            int pnum = 0;
            
            for (const auto& s : fFullPaths) {
                // Using snprintf since Teensy does not have the std::to_string function
                snprintf(num_buffer, 16, "%d", pnum++);
                std::string u = "/P" + std::string(num_buffer) + str2ID(remove0x00(s));
                uniquePaths.push_back(u);
                unique2full[u] = s;  // remember the full path associated to a unique path
            }
        
            std::map<std::string, int> uniquePath2level;                // map path to level
            for (const auto& s : uniquePaths) uniquePath2level[s] = 1;   // we init all levels to 1
            bool have_collisions = true;
        
            while (have_collisions) {
                // compute collision list
                std::set<std::string>              collisionSet;
                std::map<std::string, std::string> short2full;
                have_collisions = false;
                for (const auto& it : uniquePath2level) {
                    std::string u = it.first;
                    int n = it.second;
                    std::string shortName = cut(u, n);
                    auto p = short2full.find(shortName);
                    if (p == short2full.end()) {
                        // no collision
                        short2full[shortName] = u;
                    } else {
                        // we have a collision, add the two paths to the collision set
                        have_collisions = true;
                        collisionSet.insert(u);
                        collisionSet.insert(p->second);
                    }
                }
                for (const auto& s : collisionSet) uniquePath2level[s]++;  // increase level of colliding path
            }
        
            for (const auto& it : uniquePath2level) {
                std::string u = it.first;
                int n = it.second;
                std::string shortName = replaceCharList(cut(u, n), {'/'}, '_');
                fFull2Short[unique2full[u]] = shortName;
            }
        }
    
        std::string replaceCharList(const std::string& str, const std::vector<char>& ch1, char ch2)
        {
            auto beg = ch1.begin();
            auto end = ch1.end();
            std::string res = str;
            for (size_t i = 0; i < str.length(); ++i) {
                if (std::find(beg, end, str[i]) != end) res[i] = ch2;
            }
            return res;
        }
     
    public:
    
        PathBuilder() {}
        virtual ~PathBuilder() {}
    
        // Return true for the first level of groups
        bool pushLabel(const std::string& label_aux)
        {
            std::string label = replaceCharList(label_aux, {'/'}, '_');
            fControlsLevel.push_back(label); return fControlsLevel.size() == 1;
        }
    
        // Return true for the last level of groups
        bool popLabel() { fControlsLevel.pop_back(); return fControlsLevel.size() == 0; }
    
        // Return a complete path built from a label
        std::string buildPath(const std::string& label_aux)
        {
            std::string label = replaceCharList(label_aux, {'/'}, '_');
            std::string res = "/";
            for (size_t i = 0; i < fControlsLevel.size(); i++) {
                res = res + fControlsLevel[i] + "/";
            }
            res += label;
            return replaceCharList(res, {' ', '#', '*', ',', '?', '[', ']', '{', '}', '(', ')'}, '_');
        }
    
        // Assuming shortnames have been built, return the shortname from a label
        std::string buildShortname(const std::string& label)
        {
            return (hasShortname()) ? fFull2Short[buildPath(label)] : "";
        }
    
        bool hasShortname() { return fFull2Short.size() > 0; }
    
};

#endif  // __PathBuilder__
/**************************  END  PathBuilder.h **************************/

/*******************************************************************************
 * MapUI : Faust User Interface.
 *
 * This class creates:
 * - a map of 'labels' and zones for each UI item.
 * - a map of unique 'shortname' (built so that they never collide) and zones for each UI item
 * - a map of complete hierarchical 'paths' and zones for each UI item
 *
 * Simple 'labels', 'shortname' and complete 'paths' (to fully discriminate between possible same
 * 'labels' at different location in the UI hierachy) can be used to access a given parameter.
 ******************************************************************************/

class FAUST_API MapUI : public UI, public PathBuilder
{
    
    protected:
    
        // Label zone map
        std::map<std::string, FAUSTFLOAT*> fLabelZoneMap;
    
        // Shortname zone map
        std::map<std::string, FAUSTFLOAT*> fShortnameZoneMap;
    
        // Full path map
        std::map<std::string, FAUSTFLOAT*> fPathZoneMap;
    
        void addZoneLabel(const std::string& label, FAUSTFLOAT* zone)
        {
            std::string path = buildPath(label);
            fFullPaths.push_back(path);
            fPathZoneMap[path] = zone;
            fLabelZoneMap[label] = zone;
        }
    
    public:
        
        MapUI() {}
        virtual ~MapUI() {}
        
        // -- widget's layouts
        void openTabBox(const char* label)
        {
            pushLabel(label);
        }
        void openHorizontalBox(const char* label)
        {
            pushLabel(label);
        }
        void openVerticalBox(const char* label)
        {
            pushLabel(label);
        }
        void closeBox()
        {
            if (popLabel()) {
                // Shortnames can be computed when all fullnames are known
                computeShortNames();
                // Fill 'shortname' map
                for (const auto& it : fFullPaths) {
                    fShortnameZoneMap[fFull2Short[it]] = fPathZoneMap[it];
                }
            }
        }
        
        // -- active widgets
        void addButton(const char* label, FAUSTFLOAT* zone)
        {
            addZoneLabel(label, zone);
        }
        void addCheckButton(const char* label, FAUSTFLOAT* zone)
        {
            addZoneLabel(label, zone);
        }
        void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            addZoneLabel(label, zone);
        }
        void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            addZoneLabel(label, zone);
        }
        void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            addZoneLabel(label, zone);
        }
        
        // -- passive widgets
        void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            addZoneLabel(label, zone);
        }
        void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            addZoneLabel(label, zone);
        }
    
        // -- soundfiles
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) {}
        
        // -- metadata declarations
        virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val)
        {}
    
        //-------------------------------------------------------------------------------
        // Public API
        //-------------------------------------------------------------------------------
    
        /**
         * Set the param value.
         *
         * @param str - the UI parameter label/shortname/path
         * @param value - the UI parameter value
         *
         */
        void setParamValue(const std::string& str, FAUSTFLOAT value)
        {
            const auto fPathZoneMapIter = fPathZoneMap.find(str);
            if (fPathZoneMapIter != fPathZoneMap.end()) {
                *fPathZoneMapIter->second = value;
                return;
            }
            
            const auto fShortnameZoneMapIter = fShortnameZoneMap.find(str);
            if (fShortnameZoneMapIter != fShortnameZoneMap.end()) {
                *fShortnameZoneMapIter->second = value;
                return;
            }
            
            const auto fLabelZoneMapIter = fLabelZoneMap.find(str);
            if (fLabelZoneMapIter != fLabelZoneMap.end()) {
                *fLabelZoneMapIter->second = value;
                return;
            }
            
            fprintf(stderr, "ERROR : setParamValue '%s' not found\n", str.c_str());
        }
        
        /**
         * Return the param value.
         *
         * @param str - the UI parameter label/shortname/path
         *
         * @return the param value.
         */
        FAUSTFLOAT getParamValue(const std::string& str)
        {
            const auto fPathZoneMapIter = fPathZoneMap.find(str);
            if (fPathZoneMapIter != fPathZoneMap.end()) {
                return *fPathZoneMapIter->second;
            }
            
            const auto fShortnameZoneMapIter = fShortnameZoneMap.find(str);
            if (fShortnameZoneMapIter != fShortnameZoneMap.end()) {
                return *fShortnameZoneMapIter->second;
            }
            
            const auto fLabelZoneMapIter = fLabelZoneMap.find(str);
            if (fLabelZoneMapIter != fLabelZoneMap.end()) {
                return *fLabelZoneMapIter->second;
            }
            
            fprintf(stderr, "ERROR : getParamValue '%s' not found\n", str.c_str());
            return 0;
        }
    
        // map access 
        std::map<std::string, FAUSTFLOAT*>& getFullpathMap() { return fPathZoneMap; }
        std::map<std::string, FAUSTFLOAT*>& getShortnameMap() { return fShortnameZoneMap; }
        std::map<std::string, FAUSTFLOAT*>& getLabelMap() { return fLabelZoneMap; }
            
        /**
         * Return the number of parameters in the UI.
         *
         * @return the number of parameters
         */
        int getParamsCount() { return int(fPathZoneMap.size()); }
        
        /**
         * Return the param path.
         *
         * @param index - the UI parameter index
         *
         * @return the param path
         */
        std::string getParamAddress(int index)
        {
            if (index < 0 || index > int(fPathZoneMap.size())) {
                return "";
            } else {
                auto it = fPathZoneMap.begin();
                while (index-- > 0 && it++ != fPathZoneMap.end()) {}
                return it->first;
            }
        }
        
        const char* getParamAddress1(int index)
        {
            if (index < 0 || index > int(fPathZoneMap.size())) {
                return nullptr;
            } else {
                auto it = fPathZoneMap.begin();
                while (index-- > 0 && it++ != fPathZoneMap.end()) {}
                return it->first.c_str();
            }
        }
    
        /**
         * Return the param shortname.
         *
         * @param index - the UI parameter index
         *
         * @return the param shortname
         */
        std::string getParamShortname(int index)
        {
            if (index < 0 || index > int(fShortnameZoneMap.size())) {
                return "";
            } else {
                auto it = fShortnameZoneMap.begin();
                while (index-- > 0 && it++ != fShortnameZoneMap.end()) {}
                return it->first;
            }
        }
        
        const char* getParamShortname1(int index)
        {
            if (index < 0 || index > int(fShortnameZoneMap.size())) {
                return nullptr;
            } else {
                auto it = fShortnameZoneMap.begin();
                while (index-- > 0 && it++ != fShortnameZoneMap.end()) {}
                return it->first.c_str();
            }
        }
    
        /**
         * Return the param label.
         *
         * @param index - the UI parameter index
         *
         * @return the param label
         */
        std::string getParamLabel(int index)
        {
            if (index < 0 || index > int(fLabelZoneMap.size())) {
                return "";
            } else {
                auto it = fLabelZoneMap.begin();
                while (index-- > 0 && it++ != fLabelZoneMap.end()) {}
                return it->first;
            }
        }
        
        const char* getParamLabel1(int index)
        {
            if (index < 0 || index > int(fLabelZoneMap.size())) {
                return nullptr;
            } else {
                auto it = fLabelZoneMap.begin();
                while (index-- > 0 && it++ != fLabelZoneMap.end()) {}
                return it->first.c_str();
            }
        }
    
        /**
         * Return the param path.
         *
         * @param zone - the UI parameter memory zone
         *
         * @return the param path
         */
        std::string getParamAddress(FAUSTFLOAT* zone)
        {
            for (const auto& it : fPathZoneMap) {
                if (it.second == zone) return it.first;
            }
            return "";
        }
    
        /**
         * Return the param memory zone.
         *
         * @param zone - the UI parameter label/shortname/path
         *
         * @return the param path
         */
        FAUSTFLOAT* getParamZone(const std::string& str)
        {
            const auto fPathZoneMapIter = fPathZoneMap.find(str);
            if (fPathZoneMapIter != fPathZoneMap.end()) {
                return fPathZoneMapIter->second;
            }
            
            const auto fShortnameZoneMapIter = fShortnameZoneMap.find(str);
            if (fShortnameZoneMapIter != fShortnameZoneMap.end()) {
                return fShortnameZoneMapIter->second;
            }
            
            const auto fLabelZoneMapIter = fLabelZoneMap.find(str);
            if (fLabelZoneMapIter != fLabelZoneMap.end()) {
                return fLabelZoneMapIter->second;
            }

            return nullptr;
        }
    
        /**
         * Return the param memory zone.
         *
         * @param zone - the UI parameter index
         *
         * @return the param path
         */
        FAUSTFLOAT* getParamZone(int index)
        {
            if (index < 0 || index > int(fPathZoneMap.size())) {
                return nullptr;
            } else {
                auto it = fPathZoneMap.begin();
                while (index-- > 0 && it++ != fPathZoneMap.end()) {}
                return it->second;
            }
        }
    
        static bool endsWith(const std::string& str, const std::string& end)
        {
            size_t l1 = str.length();
            size_t l2 = end.length();
            return (l1 >= l2) && (0 == str.compare(l1 - l2, l2, end));
        }
    
};

#endif // FAUST_MAPUI_H
/**************************  END  MapUI.h **************************/
/************************** BEGIN meta.h *******************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __meta__
#define __meta__


/**
 The base class of Meta handler to be used in dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
 */
struct FAUST_API Meta {
    virtual ~Meta() {}
    virtual void declare(const char* key, const char* value) = 0;
};

#endif
/**************************  END  meta.h **************************/
/************************** BEGIN dsp.h ********************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __dsp__
#define __dsp__

#include <string>
#include <vector>
#include <cstdint>


#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

struct FAUST_API UI;
struct FAUST_API Meta;

/**
 * DSP memory manager.
 */

struct FAUST_API dsp_memory_manager {
    
    virtual ~dsp_memory_manager() {}
    
    /**
     * Inform the Memory Manager with the number of expected memory zones.
     * @param count - the number of expected memory zones
     */
    virtual void begin(size_t /*count*/) {}
    
    /**
     * Give the Memory Manager information on a given memory zone.
     * @param size - the size in bytes of the memory zone
     * @param reads - the number of Read access to the zone used to compute one frame
     * @param writes - the number of Write access to the zone used to compute one frame
     */
    virtual void info(size_t /*size*/, size_t /*reads*/, size_t /*writes*/) {}

    /**
     * Inform the Memory Manager that all memory zones have been described,
     * to possibly start a 'compute the best allocation strategy' step.
     */
    virtual void end() {}
    
    /**
     * Allocate a memory zone.
     * @param size - the memory zone size in bytes
     */
    virtual void* allocate(size_t size) = 0;
    
    /**
     * Destroy a memory zone.
     * @param ptr - the memory zone pointer to be deallocated
     */
    virtual void destroy(void* ptr) = 0;
    
};

/**
* Signal processor definition.
*/

class FAUST_API dsp {

    public:

        dsp() {}
        virtual ~dsp() {}

        /* Return instance number of audio inputs */
        virtual int getNumInputs() = 0;
    
        /* Return instance number of audio outputs */
        virtual int getNumOutputs() = 0;
    
        /**
         * Trigger the ui_interface parameter with instance specific calls
         * to 'openTabBox', 'addButton', 'addVerticalSlider'... in order to build the UI.
         *
         * @param ui_interface - the user interface builder
         */
        virtual void buildUserInterface(UI* ui_interface) = 0;
    
        /* Return the sample rate currently used by the instance */
        virtual int getSampleRate() = 0;
    
        /**
         * Global init, calls the following methods:
         * - static class 'classInit': static tables initialization
         * - 'instanceInit': constants and instance state initialization
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void init(int sample_rate) = 0;

        /**
         * Init instance state.
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void instanceInit(int sample_rate) = 0;
    
        /**
         * Init instance constant state.
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void instanceConstants(int sample_rate) = 0;
    
        /* Init default control parameters values */
        virtual void instanceResetUserInterface() = 0;
    
        /* Init instance state (like delay lines...) but keep the control parameter values */
        virtual void instanceClear() = 0;
 
        /**
         * Return a clone of the instance.
         *
         * @return a copy of the instance on success, otherwise a null pointer.
         */
        virtual ::dsp* clone() = 0;
    
        /**
         * Trigger the Meta* m parameter with instance specific calls to 'declare' (key, value) metadata.
         *
         * @param m - the Meta* meta user
         */
        virtual void metadata(Meta* m) = 0;
    
        /**
         * Read all controllers (buttons, sliders, etc.), and update the DSP state to be used by 'frame' or 'compute'.
         * This method will be filled with the -ec (--external-control) option.
         */
        virtual void control() {}
    
        /**
         * DSP instance computation to process one single frame.
         *
         * Note that by default inputs and outputs buffers are supposed to be distinct memory zones,
         * so one cannot safely write frame(inputs, inputs).
         * The -inpl option can be used for that, but only in scalar mode for now.
         * This method will be filled with the -os (--one-sample) option.
         *
         * @param inputs - the input audio buffers as an array of FAUSTFLOAT samples (eiher float, double or quad)
         * @param outputs - the output audio buffers as an array of FAUSTFLOAT samples (eiher float, double or quad)
         */
        virtual void frame(FAUSTFLOAT* inputs, FAUSTFLOAT* outputs) {}
        
        /**
         * DSP instance computation to be called with successive in/out audio buffers.
         *
         * Note that by default inputs and outputs buffers are supposed to be distinct memory zones,
         * so one cannot safely write compute(count, inputs, inputs).
         * The -inpl compilation option can be used for that, but only in scalar mode for now.
         *
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT buffers
         * (containing either float, double or quad samples)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT buffers
         * (containing either float, double or quad samples)
         */
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) = 0;
    
        /**
         * Alternative DSP instance computation method for use by subclasses, incorporating an additional `date_usec` parameter,
         * which specifies the timestamp of the first sample in the audio buffers.
         *
         * @param date_usec - the timestamp in microsec given by audio driver. By convention timestamp of -1 means 'no timestamp conversion',
         * events already have a timestamp expressed in frames.
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         */
        virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
       
};

/**
 * Generic DSP decorator.
 */

class FAUST_API decorator_dsp : public ::dsp {

    protected:

        ::dsp* fDSP;

    public:

        decorator_dsp(::dsp* dsp = nullptr):fDSP(dsp) {}
        virtual ~decorator_dsp() { delete fDSP; }

        virtual int getNumInputs() { return fDSP->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP->getNumOutputs(); }
        virtual void buildUserInterface(UI* ui_interface) { fDSP->buildUserInterface(ui_interface); }
        virtual int getSampleRate() { return fDSP->getSampleRate(); }
        virtual void init(int sample_rate) { fDSP->init(sample_rate); }
        virtual void instanceInit(int sample_rate) { fDSP->instanceInit(sample_rate); }
        virtual void instanceConstants(int sample_rate) { fDSP->instanceConstants(sample_rate); }
        virtual void instanceResetUserInterface() { fDSP->instanceResetUserInterface(); }
        virtual void instanceClear() { fDSP->instanceClear(); }
        virtual decorator_dsp* clone() { return new decorator_dsp(fDSP->clone()); }
        virtual void metadata(Meta* m) { fDSP->metadata(m); }
        // Beware: subclasses usually have to overload the two 'compute' methods
        virtual void control() { fDSP->control(); }
        virtual void frame(FAUSTFLOAT* inputs, FAUSTFLOAT* outputs) { fDSP->frame(inputs, outputs); }
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(count, inputs, outputs); }
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(date_usec, count, inputs, outputs); }
    
};

/**
 * DSP factory class, used with LLVM and Interpreter backends
 * to create DSP instances from a compiled DSP program.
 */

class FAUST_API dsp_factory {
    
    protected:
    
        // So that to force sub-classes to use deleteDSPFactory(dsp_factory* factory);
        virtual ~dsp_factory() {}
    
    public:
    
        /* Return factory name */
        virtual std::string getName() = 0;
    
        /* Return factory SHA key */
        virtual std::string getSHAKey() = 0;
    
        /* Return factory expanded DSP code */
        virtual std::string getDSPCode() = 0;
    
        /* Return factory compile options */
        virtual std::string getCompileOptions() = 0;
    
        /* Get the Faust DSP factory list of library dependancies */
        virtual std::vector<std::string> getLibraryList() = 0;
    
        /* Get the list of all used includes */
        virtual std::vector<std::string> getIncludePathnames() = 0;
    
        /* Get warning messages list for a given compilation */
        virtual std::vector<std::string> getWarningMessages() = 0;
    
        /* Create a new DSP instance, to be deleted with C++ 'delete' */
        virtual ::dsp* createDSPInstance() = 0;
    
        /* Static tables initialization, possibly implemened in sub-classes*/
        virtual void classInit(int sample_rate) {};
    
        /* Set a custom memory manager to be used when creating instances */
        virtual void setMemoryManager(dsp_memory_manager* manager) = 0;
    
        /* Return the currently set custom memory manager */
        virtual dsp_memory_manager* getMemoryManager() = 0;
    
};

// Denormal handling

#if defined (__SSE__)
#include <xmmintrin.h>
#endif

class FAUST_API ScopedNoDenormals {
    
    private:
    
        intptr_t fpsr = 0;
        
        void setFpStatusRegister(intptr_t fpsr_aux) noexcept
        {
        #if defined (__arm64__) || defined (__aarch64__)
            asm volatile("msr fpcr, %0" : : "ri" (fpsr_aux));
        #elif defined (__SSE__)
            // The volatile keyword here is needed to workaround a bug in AppleClang 13.0
            // which aggressively optimises away the variable otherwise
            volatile uint32_t fpsr_w = static_cast<uint32_t>(fpsr_aux);
            _mm_setcsr(fpsr_w);
        #endif
        }
        
        void getFpStatusRegister() noexcept
        {
        #if defined (__arm64__) || defined (__aarch64__)
            asm volatile("mrs %0, fpcr" : "=r" (fpsr));
        #elif defined (__SSE__)
            fpsr = static_cast<intptr_t>(_mm_getcsr());
        #endif
        }
    
    public:
    
        ScopedNoDenormals() noexcept
        {
        #if defined (__arm64__) || defined (__aarch64__)
            intptr_t mask = (1 << 24 /* FZ */);
        #elif defined (__SSE__)
        #if defined (__SSE2__)
            intptr_t mask = 0x8040;
        #else
            intptr_t mask = 0x8000;
        #endif
        #else
            intptr_t mask = 0x0000;
        #endif
            getFpStatusRegister();
            setFpStatusRegister(fpsr | mask);
        }
        
        ~ScopedNoDenormals() noexcept
        {
            setFpStatusRegister(fpsr);
        }

};

#define AVOIDDENORMALS ScopedNoDenormals ftz_scope;

#endif

/************************** END dsp.h **************************/

// BEGIN-FAUSTDSP


#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS Macalla
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

#if defined(_WIN32)
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
#endif

class MacallaSIG0 {
	
  private:
	
	int iVec1[2];
	int iRec18[2];
	
  public:
	
	int getNumInputsMacallaSIG0() {
		return 0;
	}
	int getNumOutputsMacallaSIG0() {
		return 1;
	}
	
	void instanceInitMacallaSIG0(int sample_rate) {
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			iVec1[l11] = 0;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			iRec18[l12] = 0;
		}
	}
	
	void fillMacallaSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = i1 + 1) {
			iVec1[0] = 1;
			iRec18[0] = (iVec1[1] + iRec18[1]) % 65536;
			table[i1] = std::sin(9.58738e-05f * float(iRec18[0]));
			iVec1[1] = iVec1[0];
			iRec18[1] = iRec18[0];
		}
	}

};

static MacallaSIG0* newMacallaSIG0() { return (MacallaSIG0*)new MacallaSIG0(); }
static void deleteMacallaSIG0(MacallaSIG0* dsp) { delete dsp; }

static float ftbl0MacallaSIG0[65536];
static float Macalla_faustpower2_f(float value) {
	return value * value;
}

class Macalla : public dsp {
	
 private:
	
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider0;
	float fConst2;
	float fRec1_perm[4];
	FAUSTFLOAT fHslider1;
	float fRec2_perm[4];
	FAUSTFLOAT fHslider2;
	float fRec3_perm[4];
	float fConst3;
	FAUSTFLOAT fHslider3;
	float fRec11_perm[4];
	int iRec13_perm[4];
	int iVec0_perm[4];
	float fConst4;
	float fRec14_perm[4];
	float fYec0_perm[4];
	float fRec12_perm[4];
	float fRec16_perm[4];
	float fConst5;
	float fRec17_perm[4];
	float fRec15_perm[4];
	float fConst6;
	float fConst7;
	float fConst8;
	float fConst9;
	float fConst10;
	float fRec10_perm[4];
	int iRec20_perm[4];
	int iConst11;
	float fRec19_perm[4];
	FAUSTFLOAT fCheckbox0;
	float fConst12;
	float fConst13;
	float fRec22_perm[4];
	float fConst14;
	float fConst15;
	float fRec21_perm[4];
	float fConst16;
	float fConst17;
	float fRec23_perm[4];
	FAUSTFLOAT fHslider4;
	float fRec24_perm[4];
	float fRec26_perm[4];
	float fRec27_perm[4];
	float fYec1_perm[4];
	float fRec28_perm[4];
	float fConst18;
	float fRec30_perm[4];
	float fConst19;
	float fYec2_perm[4];
	float fYec3[2048];
	int fYec3_idx;
	int fYec3_idx_save;
	float fConst20;
	float fConst21;
	float fConst22;
	int iConst23;
	float fConst24;
	int iConst25;
	float fRec29_perm[4];
	float fConst26;
	float fRec31_perm[4];
	float fConst27;
	float fRec32_perm[4];
	float fConst28;
	float fConst29;
	float fConst30;
	float fConst31;
	float fConst32;
	float fConst33;
	float fConst34;
	float fConst35;
	float fConst36;
	float fConst37;
	float fConst38;
	float fRec25_perm[4];
	FAUSTFLOAT fHslider5;
	float fRec33_perm[4];
	float fConst39;
	float fConst40;
	float fRec9_perm[4];
	FAUSTFLOAT fHslider6;
	float fRec34_perm[4];
	float fRec8_perm[4];
	float fConst41;
	FAUSTFLOAT fHslider7;
	float fRec35_perm[4];
	float fConst42;
	float fRec36_perm[4];
	float fYec4[2048];
	int fYec4_idx;
	int fYec4_idx_save;
	int iConst43;
	int iConst44;
	float fConst45;
	float fConst46;
	float fRec6_perm[4];
	float fConst47;
	float fRec37_perm[4];
	float fYec5[4096];
	int fYec5_idx;
	int fYec5_idx_save;
	float fRec4_perm[4];
	FAUSTFLOAT fHslider8;
	float fRec38_perm[4];
	FAUSTFLOAT fHslider9;
	float fRec43_perm[4];
	float fRec44_perm[4];
	FAUSTFLOAT fHslider10;
	float fRec39_perm[4];
	float fRec40_perm[4];
	float fRec41_perm[4];
	float fRec42_perm[4];
	float fYec6[131072];
	int fYec6_idx;
	int fYec6_idx_save;
	float fYec7[1048576];
	int fYec7_idx;
	int fYec7_idx_save;
	float fRec0[262144];
	int fRec0_idx;
	int fRec0_idx_save;
	FAUSTFLOAT fHslider11;
	float fRec45_perm[4];
	float fConst48;
	float fConst49;
	float fRec49_perm[4];
	float fRec50_perm[4];
	float fConst50;
	float fConst51;
	float fConst52;
	float fConst53;
	float fRec48_perm[4];
	float fConst54;
	float fRec47_perm[4];
	float fRec46_perm[4];
	float fRec52_perm[4];
	float fRec51_perm[4];
	FAUSTFLOAT fHslider12;
	FAUSTFLOAT fHslider13;
	float fRec53_perm[4];
	FAUSTFLOAT fHslider14;
	float fRec54_perm[4];
	FAUSTFLOAT fHslider15;
	FAUSTFLOAT fHslider16;
	float fRec65_perm[4];
	float fRec64_perm[4];
	float fRec63_perm[4];
	float fRec67_perm[4];
	float fRec66_perm[4];
	float fRec62_perm[4];
	float fRec68_perm[4];
	float fRec70_perm[4];
	float fRec69_perm[4];
	float fRec71_perm[4];
	float fRec61_perm[4];
	float fRec60_perm[4];
	float fRec72_perm[4];
	float fYec8[16384];
	int fYec8_idx;
	int fYec8_idx_save;
	float fRec58_perm[4];
	float fRec73_perm[4];
	float fYec9[16384];
	int fYec9_idx;
	int fYec9_idx_save;
	float fRec56_perm[4];
	float fRec78_perm[4];
	float fRec74_perm[4];
	float fRec75_perm[4];
	float fRec76_perm[4];
	float fRec77_perm[4];
	float fYec10[131072];
	int fYec10_idx;
	int fYec10_idx_save;
	float fYec11[1048576];
	int fYec11_idx;
	int fYec11_idx_save;
	float fRec55[262144];
	int fRec55_idx;
	int fRec55_idx_save;
	float fRec81_perm[4];
	float fRec80_perm[4];
	float fRec79_perm[4];
	float fRec83_perm[4];
	float fRec82_perm[4];
	float fRec84_perm[4];
	
 public:
	Macalla() {
	}
	
	void metadata(Meta* m) { 
		m->declare("analyzers.lib/amp_follower_ar:author", "Jonatan Liljedahl, revised by Romain Michon");
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "1.2.0");
		m->declare("basics.lib/bypass1:author", "Julius Smith");
		m->declare("basics.lib/downSample:author", "Romain Michon");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/sAndH:author", "Romain Michon");
		m->declare("basics.lib/version", "1.21.0");
		m->declare("category", "Echo / Delay");
		m->declare("compile_options", "-a faustMinimal.h -lang cpp -i -ct 1 -cn Macalla -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 32");
		m->declare("compressors.lib/compression_gain_mono:author", "Julius O. Smith III");
		m->declare("compressors.lib/compression_gain_mono:copyright", "Copyright (C) 2014-2020 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("compressors.lib/compression_gain_mono:license", "MIT-style STK-4.3 license");
		m->declare("compressors.lib/compressor_lad_mono:author", "Julius O. Smith III");
		m->declare("compressors.lib/compressor_lad_mono:copyright", "Copyright (C) 2014-2020 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("compressors.lib/compressor_lad_mono:license", "MIT-style STK-4.3 license");
		m->declare("compressors.lib/compressor_mono:author", "Julius O. Smith III");
		m->declare("compressors.lib/compressor_mono:copyright", "Copyright (C) 2014-2020 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("compressors.lib/compressor_mono:license", "MIT-style STK-4.3 license");
		m->declare("compressors.lib/limiter_1176_R4_mono:author", "Julius O. Smith III");
		m->declare("compressors.lib/limiter_1176_R4_mono:copyright", "Copyright (C) 2014-2020 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("compressors.lib/limiter_1176_R4_mono:license", "MIT-style STK-4.3 license");
		m->declare("compressors.lib/name", "Faust Compressor Effect Library");
		m->declare("compressors.lib/peak_compression_gain_mono_db:author", "Bart Brouns");
		m->declare("compressors.lib/peak_compression_gain_mono_db:license", "GPLv3");
		m->declare("compressors.lib/ratio2strength:author", "Bart Brouns");
		m->declare("compressors.lib/ratio2strength:license", "GPLv3");
		m->declare("compressors.lib/version", "1.6.0");
		m->declare("delays.lib/fdelay1:author", "Julius O. Smith III");
		m->declare("delays.lib/fdelayltv:author", "Julius O. Smith III");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.1.0");
		m->declare("filename", "Macalla.dsp");
		m->declare("filters.lib/allpass_fcomb:author", "Julius O. Smith III");
		m->declare("filters.lib/allpass_fcomb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/allpass_fcomb:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/bandpass0_bandstop1:author", "Julius O. Smith III");
		m->declare("filters.lib/bandpass0_bandstop1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/bandpass0_bandstop1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/bandpass:author", "Julius O. Smith III");
		m->declare("filters.lib/bandpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/bandpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/highpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/nlf2:author", "Julius O. Smith III");
		m->declare("filters.lib/nlf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/nlf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/pole:author", "Julius O. Smith III");
		m->declare("filters.lib/pole:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/pole:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1sb:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1sb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1sb:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "1.7.1");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.1");
		m->declare("misceffects.lib/name", "Misc Effects Library");
		m->declare("misceffects.lib/version", "2.5.1");
		m->declare("name", "Macalla ");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "1.4.1");
		m->declare("oscillators.lib/lf_sawpos:author", "Bart Brouns, revised by Stéphane Letz");
		m->declare("oscillators.lib/lf_sawpos:licence", "STK-4.3");
		m->declare("oscillators.lib/lf_triangle:author", "Bart Brouns");
		m->declare("oscillators.lib/lf_triangle:licence", "STK-4.3");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/saw1:author", "Bart Brouns");
		m->declare("oscillators.lib/saw1:licence", "STK-4.3");
		m->declare("oscillators.lib/sawN:author", "Julius O. Smith III");
		m->declare("oscillators.lib/sawN:license", "STK-4.3");
		m->declare("oscillators.lib/version", "1.6.0");
		m->declare("phaflangers.lib/name", "Faust Phaser and Flanger Library");
		m->declare("phaflangers.lib/version", "1.1.0");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/onePoleSwitching:author", "Jonatan Liljedahl, revised by Dario Sanfilippo");
		m->declare("signals.lib/onePoleSwitching:licence", "STK-4.3");
		m->declare("signals.lib/version", "1.6.0");
	}

	virtual int getNumInputs() {
		return 2;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
		MacallaSIG0* sig0 = newMacallaSIG0();
		sig0->instanceInitMacallaSIG0(sample_rate);
		sig0->fillMacallaSIG0(65536, ftbl0MacallaSIG0);
		deleteMacallaSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 44.1f / fConst0;
		fConst2 = 1.0f - fConst1;
		fConst3 = 30.87f / fConst0;
		fConst4 = 1.0f / fConst0;
		fConst5 = 0.1f / fConst0;
		fConst6 = std::tan(25132.742f / fConst0);
		fConst7 = 1.0f / fConst6;
		fConst8 = 1.0f / ((fConst7 + 1.4142135f) / fConst6 + 1.0f);
		fConst9 = (fConst7 + -1.4142135f) / fConst6 + 1.0f;
		fConst10 = 2.0f * (1.0f - 1.0f / Macalla_faustpower2_f(fConst6));
		iConst11 = int(8.333333e-05f * fConst0);
		fConst12 = std::exp(-(2.0f / fConst0));
		fConst13 = std::exp(-(1.25e+03f / fConst0));
		fConst14 = std::exp(-(2.5e+03f / fConst0));
		fConst15 = 0.75f * (1.0f - fConst14);
		fConst16 = 2e+01f / fConst0;
		fConst17 = std::exp(-fConst16);
		fConst18 = 6e+01f / fConst0;
		fConst19 = 0.004166667f * fConst0;
		fConst20 = std::max<float>(0.0f, std::min<float>(2047.0f, 0.008333334f * fConst0));
		fConst21 = std::floor(fConst20);
		fConst22 = fConst21 + (1.0f - fConst20);
		iConst23 = int(fConst20);
		fConst24 = fConst20 - fConst21;
		iConst25 = iConst23 + 1;
		fConst26 = 528.0f / fConst0;
		fConst27 = 1.1e+02f / fConst0;
		fConst28 = 48.0f / fConst0;
		fConst29 = std::tan(31415.926f / fConst0);
		fConst30 = Macalla_faustpower2_f(std::sqrt(4.0f * Macalla_faustpower2_f(fConst0) * std::tan(1570.7964f / fConst0) * fConst29));
		fConst31 = Macalla_faustpower2_f(fConst4) * fConst30;
		fConst32 = fConst0 * fConst29;
		fConst33 = 2.0f * fConst32 - 0.5f * (fConst30 / fConst32);
		fConst34 = 2.0f * (fConst33 / fConst0);
		fConst35 = fConst31 + fConst34 + 4.0f;
		fConst36 = 1.0f / fConst35;
		fConst37 = 2.0f * fConst31 + -8.0f;
		fConst38 = fConst31 + (4.0f - fConst34);
		fConst39 = 2.0f * (fConst33 / (fConst0 * fConst35));
		fConst40 = 3.1415927f / fConst0;
		fConst41 = 41.895f / fConst0;
		fConst42 = 0.15f / fConst0;
		iConst43 = int(0.05f * fConst0);
		iConst44 = iConst43 + 2;
		fConst45 = float(iConst43);
		fConst46 = 0.001f * fConst0;
		fConst47 = 0.23f / fConst0;
		fConst48 = std::sin(fConst40);
		fConst49 = std::cos(fConst40);
		fConst50 = std::exp(-(3141.5928f / fConst0));
		fConst51 = 2.0f * fConst50;
		fConst52 = 1.25f / fConst0;
		fConst53 = Macalla_faustpower2_f(fConst50);
		fConst54 = 1.5625f / fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.0f);
		fHslider1 = FAUSTFLOAT(0.0f);
		fHslider2 = FAUSTFLOAT(0.5f);
		fHslider3 = FAUSTFLOAT(0.1f);
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider4 = FAUSTFLOAT(0.1f);
		fHslider5 = FAUSTFLOAT(2.5e+02f);
		fHslider6 = FAUSTFLOAT(1e+04f);
		fHslider7 = FAUSTFLOAT(0.0f);
		fHslider8 = FAUSTFLOAT(0.0f);
		fHslider9 = FAUSTFLOAT(0.0f);
		fHslider10 = FAUSTFLOAT(5e+02f);
		fHslider11 = FAUSTFLOAT(1.0f);
		fHslider12 = FAUSTFLOAT(2e+02f);
		fHslider13 = FAUSTFLOAT(1e+01f);
		fHslider14 = FAUSTFLOAT(0.0f);
		fHslider15 = FAUSTFLOAT(5e+01f);
		fHslider16 = FAUSTFLOAT(0.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 4; l0 = l0 + 1) {
			fRec1_perm[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 4; l1 = l1 + 1) {
			fRec2_perm[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 4; l2 = l2 + 1) {
			fRec3_perm[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 4; l3 = l3 + 1) {
			fRec11_perm[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 4; l4 = l4 + 1) {
			iRec13_perm[l4] = 0;
		}
		for (int l5 = 0; l5 < 4; l5 = l5 + 1) {
			iVec0_perm[l5] = 0;
		}
		for (int l6 = 0; l6 < 4; l6 = l6 + 1) {
			fRec14_perm[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 4; l7 = l7 + 1) {
			fYec0_perm[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 4; l8 = l8 + 1) {
			fRec12_perm[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 4; l9 = l9 + 1) {
			fRec16_perm[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 4; l10 = l10 + 1) {
			fRec17_perm[l10] = 0.0f;
		}
		for (int l13 = 0; l13 < 4; l13 = l13 + 1) {
			fRec15_perm[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 4; l14 = l14 + 1) {
			fRec10_perm[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 4; l15 = l15 + 1) {
			iRec20_perm[l15] = 0;
		}
		for (int l16 = 0; l16 < 4; l16 = l16 + 1) {
			fRec19_perm[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 4; l17 = l17 + 1) {
			fRec22_perm[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 4; l18 = l18 + 1) {
			fRec21_perm[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 4; l19 = l19 + 1) {
			fRec23_perm[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 4; l20 = l20 + 1) {
			fRec24_perm[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 4; l21 = l21 + 1) {
			fRec26_perm[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 4; l22 = l22 + 1) {
			fRec27_perm[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 4; l23 = l23 + 1) {
			fYec1_perm[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 4; l24 = l24 + 1) {
			fRec28_perm[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 4; l25 = l25 + 1) {
			fRec30_perm[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 4; l26 = l26 + 1) {
			fYec2_perm[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 2048; l27 = l27 + 1) {
			fYec3[l27] = 0.0f;
		}
		fYec3_idx = 0;
		fYec3_idx_save = 0;
		for (int l28 = 0; l28 < 4; l28 = l28 + 1) {
			fRec29_perm[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 4; l29 = l29 + 1) {
			fRec31_perm[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 4; l30 = l30 + 1) {
			fRec32_perm[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 4; l31 = l31 + 1) {
			fRec25_perm[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 4; l32 = l32 + 1) {
			fRec33_perm[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 4; l33 = l33 + 1) {
			fRec9_perm[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 4; l34 = l34 + 1) {
			fRec34_perm[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 4; l35 = l35 + 1) {
			fRec8_perm[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 4; l36 = l36 + 1) {
			fRec35_perm[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 4; l37 = l37 + 1) {
			fRec36_perm[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 2048; l38 = l38 + 1) {
			fYec4[l38] = 0.0f;
		}
		fYec4_idx = 0;
		fYec4_idx_save = 0;
		for (int l39 = 0; l39 < 4; l39 = l39 + 1) {
			fRec6_perm[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 4; l40 = l40 + 1) {
			fRec37_perm[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 4096; l41 = l41 + 1) {
			fYec5[l41] = 0.0f;
		}
		fYec5_idx = 0;
		fYec5_idx_save = 0;
		for (int l42 = 0; l42 < 4; l42 = l42 + 1) {
			fRec4_perm[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 4; l43 = l43 + 1) {
			fRec38_perm[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 4; l44 = l44 + 1) {
			fRec43_perm[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 4; l45 = l45 + 1) {
			fRec44_perm[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 4; l46 = l46 + 1) {
			fRec39_perm[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 4; l47 = l47 + 1) {
			fRec40_perm[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 4; l48 = l48 + 1) {
			fRec41_perm[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 4; l49 = l49 + 1) {
			fRec42_perm[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 131072; l50 = l50 + 1) {
			fYec6[l50] = 0.0f;
		}
		fYec6_idx = 0;
		fYec6_idx_save = 0;
		for (int l51 = 0; l51 < 1048576; l51 = l51 + 1) {
			fYec7[l51] = 0.0f;
		}
		fYec7_idx = 0;
		fYec7_idx_save = 0;
		for (int l52 = 0; l52 < 262144; l52 = l52 + 1) {
			fRec0[l52] = 0.0f;
		}
		fRec0_idx = 0;
		fRec0_idx_save = 0;
		for (int l53 = 0; l53 < 4; l53 = l53 + 1) {
			fRec45_perm[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 4; l54 = l54 + 1) {
			fRec49_perm[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 4; l55 = l55 + 1) {
			fRec50_perm[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 4; l56 = l56 + 1) {
			fRec48_perm[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 4; l57 = l57 + 1) {
			fRec47_perm[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 4; l58 = l58 + 1) {
			fRec46_perm[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 4; l59 = l59 + 1) {
			fRec52_perm[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 4; l60 = l60 + 1) {
			fRec51_perm[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 4; l61 = l61 + 1) {
			fRec53_perm[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 4; l62 = l62 + 1) {
			fRec54_perm[l62] = 0.0f;
		}
		for (int l63 = 0; l63 < 4; l63 = l63 + 1) {
			fRec65_perm[l63] = 0.0f;
		}
		for (int l64 = 0; l64 < 4; l64 = l64 + 1) {
			fRec64_perm[l64] = 0.0f;
		}
		for (int l65 = 0; l65 < 4; l65 = l65 + 1) {
			fRec63_perm[l65] = 0.0f;
		}
		for (int l66 = 0; l66 < 4; l66 = l66 + 1) {
			fRec67_perm[l66] = 0.0f;
		}
		for (int l67 = 0; l67 < 4; l67 = l67 + 1) {
			fRec66_perm[l67] = 0.0f;
		}
		for (int l68 = 0; l68 < 4; l68 = l68 + 1) {
			fRec62_perm[l68] = 0.0f;
		}
		for (int l69 = 0; l69 < 4; l69 = l69 + 1) {
			fRec68_perm[l69] = 0.0f;
		}
		for (int l70 = 0; l70 < 4; l70 = l70 + 1) {
			fRec70_perm[l70] = 0.0f;
		}
		for (int l71 = 0; l71 < 4; l71 = l71 + 1) {
			fRec69_perm[l71] = 0.0f;
		}
		for (int l72 = 0; l72 < 4; l72 = l72 + 1) {
			fRec71_perm[l72] = 0.0f;
		}
		for (int l73 = 0; l73 < 4; l73 = l73 + 1) {
			fRec61_perm[l73] = 0.0f;
		}
		for (int l74 = 0; l74 < 4; l74 = l74 + 1) {
			fRec60_perm[l74] = 0.0f;
		}
		for (int l75 = 0; l75 < 4; l75 = l75 + 1) {
			fRec72_perm[l75] = 0.0f;
		}
		for (int l76 = 0; l76 < 16384; l76 = l76 + 1) {
			fYec8[l76] = 0.0f;
		}
		fYec8_idx = 0;
		fYec8_idx_save = 0;
		for (int l77 = 0; l77 < 4; l77 = l77 + 1) {
			fRec58_perm[l77] = 0.0f;
		}
		for (int l78 = 0; l78 < 4; l78 = l78 + 1) {
			fRec73_perm[l78] = 0.0f;
		}
		for (int l79 = 0; l79 < 16384; l79 = l79 + 1) {
			fYec9[l79] = 0.0f;
		}
		fYec9_idx = 0;
		fYec9_idx_save = 0;
		for (int l80 = 0; l80 < 4; l80 = l80 + 1) {
			fRec56_perm[l80] = 0.0f;
		}
		for (int l81 = 0; l81 < 4; l81 = l81 + 1) {
			fRec78_perm[l81] = 0.0f;
		}
		for (int l82 = 0; l82 < 4; l82 = l82 + 1) {
			fRec74_perm[l82] = 0.0f;
		}
		for (int l83 = 0; l83 < 4; l83 = l83 + 1) {
			fRec75_perm[l83] = 0.0f;
		}
		for (int l84 = 0; l84 < 4; l84 = l84 + 1) {
			fRec76_perm[l84] = 0.0f;
		}
		for (int l85 = 0; l85 < 4; l85 = l85 + 1) {
			fRec77_perm[l85] = 0.0f;
		}
		for (int l86 = 0; l86 < 131072; l86 = l86 + 1) {
			fYec10[l86] = 0.0f;
		}
		fYec10_idx = 0;
		fYec10_idx_save = 0;
		for (int l87 = 0; l87 < 1048576; l87 = l87 + 1) {
			fYec11[l87] = 0.0f;
		}
		fYec11_idx = 0;
		fYec11_idx_save = 0;
		for (int l88 = 0; l88 < 262144; l88 = l88 + 1) {
			fRec55[l88] = 0.0f;
		}
		fRec55_idx = 0;
		fRec55_idx_save = 0;
		for (int l89 = 0; l89 < 4; l89 = l89 + 1) {
			fRec81_perm[l89] = 0.0f;
		}
		for (int l90 = 0; l90 < 4; l90 = l90 + 1) {
			fRec80_perm[l90] = 0.0f;
		}
		for (int l91 = 0; l91 < 4; l91 = l91 + 1) {
			fRec79_perm[l91] = 0.0f;
		}
		for (int l92 = 0; l92 < 4; l92 = l92 + 1) {
			fRec83_perm[l92] = 0.0f;
		}
		for (int l93 = 0; l93 < 4; l93 = l93 + 1) {
			fRec82_perm[l93] = 0.0f;
		}
		for (int l94 = 0; l94 < 4; l94 = l94 + 1) {
			fRec84_perm[l94] = 0.0f;
		}
	}
	
	virtual void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	
	virtual void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual Macalla* clone() {
		return new Macalla();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("Macalla ");
		ui_interface->addCheckButton("bitcrush", &fCheckbox0);
		ui_interface->declare(&fHslider14, "unit", "percent");
		ui_interface->addHorizontalSlider("ducking_amount", &fHslider14, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider9, "unit", "percent");
		ui_interface->addHorizontalSlider("glitch", &fHslider9, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider4, "unit", "percent");
		ui_interface->addHorizontalSlider("tape_noise", &fHslider4, FAUSTFLOAT(0.1f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider3, "unit", "percent");
		ui_interface->addHorizontalSlider("wow_flutter", &fHslider3, FAUSTFLOAT(0.1f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("delaytime", &fHslider10, FAUSTFLOAT(5e+02f), FAUSTFLOAT(1.0f), FAUSTFLOAT(5e+03f), FAUSTFLOAT(0.1f));
		ui_interface->addHorizontalSlider("diffusion", &fHslider7, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("duck_attack", &fHslider13, FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("duck_release", &fHslider12, FAUSTFLOAT(2e+02f), FAUSTFLOAT(5e+01f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("feedback", &fHslider2, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("highpass", &fHslider5, FAUSTFLOAT(2.5e+02f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(2e+04f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("lowpass", &fHslider6, FAUSTFLOAT(1e+04f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(2e+04f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("mix", &fHslider15, FAUSTFLOAT(5e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("output_gain", &fHslider0, FAUSTFLOAT(0.0f), FAUSTFLOAT(-96.0f), FAUSTFLOAT(12.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("phaser_depth", &fHslider11, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("pitch_shift", &fHslider8, FAUSTFLOAT(0.0f), FAUSTFLOAT(-12.0f), FAUSTFLOAT(12.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("pitch_shift2", &fHslider1, FAUSTFLOAT(0.0f), FAUSTFLOAT(-12.0f), FAUSTFLOAT(12.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("spread_amount", &fHslider16, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0_ptr = inputs[0];
		FAUSTFLOAT* input1_ptr = inputs[1];
		FAUSTFLOAT* output0_ptr = outputs[0];
		FAUSTFLOAT* output1_ptr = outputs[1];
		float fSlow0 = fConst1 * float(fHslider0);
		float fRec1_tmp[36];
		float* fRec1 = &fRec1_tmp[4];
		float fSlow1 = float(fHslider1);
		float fSlow2 = std::pow(2.0f, 0.083333336f * fSlow1);
		float fRec2_tmp[36];
		float* fRec2 = &fRec2_tmp[4];
		float fSlow3 = fConst1 * float(fHslider2);
		float fRec3_tmp[36];
		float* fRec3 = &fRec3_tmp[4];
		float fSlow4 = fConst3 * float(fHslider3);
		float fRec11_tmp[36];
		float* fRec11 = &fRec11_tmp[4];
		int iRec13_tmp[36];
		int* iRec13 = &iRec13_tmp[4];
		int iVec0_tmp[36];
		int* iVec0 = &iVec0_tmp[4];
		int iZec0[32];
		float fZec1[32];
		float fZec2[32];
		float fRec14_tmp[36];
		float* fRec14 = &fRec14_tmp[4];
		float fYec0_tmp[36];
		float* fYec0 = &fYec0_tmp[4];
		float fZec3[32];
		float fRec12_tmp[36];
		float* fRec12 = &fRec12_tmp[4];
		float fZec4[32];
		float fZec5[32];
		float fRec16_tmp[36];
		float* fRec16 = &fRec16_tmp[4];
		float fZec6[32];
		float fRec17_tmp[36];
		float* fRec17 = &fRec17_tmp[4];
		float fZec7[32];
		float fRec15_tmp[36];
		float* fRec15 = &fRec15_tmp[4];
		float fZec8[32];
		float fZec9[32];
		float fZec10[32];
		int iZec11[32];
		float fRec10_tmp[36];
		float* fRec10 = &fRec10_tmp[4];
		int iRec20_tmp[36];
		int* iRec20 = &iRec20_tmp[4];
		int iZec12[32];
		float fZec13[32];
		float fRec19_tmp[36];
		float* fRec19 = &fRec19_tmp[4];
		int iSlow5 = int(float(fCheckbox0));
		float fZec14[32];
		float fZec15[32];
		float fZec16[32];
		float fRec22_tmp[36];
		float* fRec22 = &fRec22_tmp[4];
		float fRec21_tmp[36];
		float* fRec21 = &fRec21_tmp[4];
		float fZec17[32];
		float fZec18[32];
		float fZec19[32];
		float fRec23_tmp[36];
		float* fRec23 = &fRec23_tmp[4];
		float fSlow6 = fConst1 * float(fHslider4);
		float fRec24_tmp[36];
		float* fRec24 = &fRec24_tmp[4];
		float fRec26_tmp[36];
		float* fRec26 = &fRec26_tmp[4];
		float fZec20[32];
		float fRec27_tmp[36];
		float* fRec27 = &fRec27_tmp[4];
		float fYec1_tmp[36];
		float* fYec1 = &fYec1_tmp[4];
		int iZec21[32];
		float fRec28_tmp[36];
		float* fRec28 = &fRec28_tmp[4];
		float fZec22[32];
		float fRec30_tmp[36];
		float* fRec30 = &fRec30_tmp[4];
		float fYec2_tmp[36];
		float* fYec2 = &fYec2_tmp[4];
		float fRec29_tmp[36];
		float* fRec29 = &fRec29_tmp[4];
		float fZec23[32];
		float fRec31_tmp[36];
		float* fRec31 = &fRec31_tmp[4];
		float fZec24[32];
		float fRec32_tmp[36];
		float* fRec32 = &fRec32_tmp[4];
		float fZec25[32];
		float fRec25_tmp[36];
		float* fRec25 = &fRec25_tmp[4];
		float fSlow7 = fConst1 * float(fHslider5);
		float fRec33_tmp[36];
		float* fRec33 = &fRec33_tmp[4];
		float fZec26[32];
		float fZec27[32];
		float fZec28[32];
		float fZec29[32];
		float fZec30[32];
		float fZec31[32];
		float fZec32[32];
		float fZec33[32];
		float fRec9_tmp[36];
		float* fRec9 = &fRec9_tmp[4];
		float fSlow8 = fConst1 * float(fHslider6);
		float fRec34_tmp[36];
		float* fRec34 = &fRec34_tmp[4];
		float fZec34[32];
		float fZec35[32];
		float fZec36[32];
		float fZec37[32];
		float fZec38[32];
		float fZec39[32];
		float fRec8_tmp[36];
		float* fRec8 = &fRec8_tmp[4];
		float fSlow9 = fConst41 * float(fHslider7);
		float fRec35_tmp[36];
		float* fRec35 = &fRec35_tmp[4];
		float fZec40[32];
		float fRec36_tmp[36];
		float* fRec36 = &fRec36_tmp[4];
		float fZec41[32];
		float fZec42[32];
		int iZec43[32];
		float fZec44[32];
		float fRec6_tmp[36];
		float* fRec6 = &fRec6_tmp[4];
		float fRec7[32];
		float fZec45[32];
		float fRec37_tmp[36];
		float* fRec37 = &fRec37_tmp[4];
		float fZec46[32];
		float fZec47[32];
		int iZec48[32];
		float fZec49[32];
		float fRec4_tmp[36];
		float* fRec4 = &fRec4_tmp[4];
		float fRec5[32];
		float fSlow10 = float(fHslider8);
		float fSlow11 = std::pow(2.0f, 0.083333336f * fSlow10);
		float fRec38_tmp[36];
		float* fRec38 = &fRec38_tmp[4];
		float fSlow12 = fConst1 * float(fHslider9);
		float fRec43_tmp[36];
		float* fRec43 = &fRec43_tmp[4];
		float fZec50[32];
		float fZec51[32];
		float fRec44_tmp[36];
		float* fRec44 = &fRec44_tmp[4];
		float fSlow13 = float(fHslider10);
		float fSlow14 = 0.001f * fSlow13;
		float fZec52[32];
		float fZec53[32];
		float fRec39_tmp[36];
		float* fRec39 = &fRec39_tmp[4];
		float fRec40_tmp[36];
		float* fRec40 = &fRec40_tmp[4];
		float fRec41_tmp[36];
		float* fRec41 = &fRec41_tmp[4];
		float fRec42_tmp[36];
		float* fRec42 = &fRec42_tmp[4];
		int iSlow15 = fSlow1 != 0.0f;
		float fZec54[32];
		int iZec55[32];
		int iZec56[32];
		float fZec57[32];
		float fZec58[32];
		float fZec59[32];
		int iZec60[32];
		float fZec61[32];
		float fZec62[32];
		int iZec63[32];
		int iZec64[32];
		float fZec65[32];
		float fZec66[32];
		float fZec67[32];
		int iZec68[32];
		float fZec69[32];
		int iSlow16 = fSlow10 != 0.0f;
		int iZec70[32];
		int iZec71[32];
		float fZec72[32];
		float fZec73[32];
		float fZec74[32];
		int iZec75[32];
		float fZec76[32];
		float fZec77[32];
		int iZec78[32];
		int iZec79[32];
		float fZec80[32];
		float fZec81[32];
		float fZec82[32];
		int iZec83[32];
		float fZec84[32];
		float fZec85[32];
		float fSlow17 = fConst1 * float(fHslider11);
		float fRec45_tmp[36];
		float* fRec45 = &fRec45_tmp[4];
		float fRec49_tmp[36];
		float* fRec49 = &fRec49_tmp[4];
		float fRec50_tmp[36];
		float* fRec50 = &fRec50_tmp[4];
		float fZec86[32];
		float fZec87[32];
		float fZec88[32];
		float fRec48_tmp[36];
		float* fRec48 = &fRec48_tmp[4];
		float fZec89[32];
		float fZec90[32];
		float fRec47_tmp[36];
		float* fRec47 = &fRec47_tmp[4];
		float fRec46_tmp[36];
		float* fRec46 = &fRec46_tmp[4];
		float fZec91[32];
		float fZec92[32];
		float fZec93[32];
		float fZec94[32];
		float fRec52_tmp[36];
		float* fRec52 = &fRec52_tmp[4];
		float fRec51_tmp[36];
		float* fRec51 = &fRec51_tmp[4];
		float fZec95[32];
		float fSlow18 = 0.001f * float(fHslider12);
		int iSlow19 = std::fabs(fSlow18) < 1.1920929e-07f;
		float fSlow20 = ((iSlow19) ? 0.0f : std::exp(-(fConst4 / ((iSlow19) ? 1.0f : fSlow18))));
		float fSlow21 = 0.001f * float(fHslider13);
		int iSlow22 = std::fabs(fSlow21) < 1.1920929e-07f;
		float fSlow23 = ((iSlow22) ? 0.0f : std::exp(-(fConst4 / ((iSlow22) ? 1.0f : fSlow21))));
		float fZec96[32];
		float fRec53_tmp[36];
		float* fRec53 = &fRec53_tmp[4];
		float fSlow24 = fConst1 * float(fHslider14);
		float fRec54_tmp[36];
		float* fRec54 = &fRec54_tmp[4];
		float fSlow25 = 0.01f * float(fHslider15);
		float fSlow26 = 1.0f - fSlow25;
		float fZec97[32];
		float fZec98[32];
		float fZec99[32];
		float fZec100[32];
		int iZec101[32];
		float fZec102[32];
		float fZec103[32];
		float fSlow27 = fConst1 * float(fHslider16);
		float fRec65_tmp[36];
		float* fRec65 = &fRec65_tmp[4];
		float fZec104[32];
		float fZec105[32];
		float fRec64_tmp[36];
		float* fRec64 = &fRec64_tmp[4];
		float fRec63_tmp[36];
		float* fRec63 = &fRec63_tmp[4];
		float fZec106[32];
		float fRec67_tmp[36];
		float* fRec67 = &fRec67_tmp[4];
		float fRec66_tmp[36];
		float* fRec66 = &fRec66_tmp[4];
		float fZec107[32];
		float fZec108[32];
		float fZec109[32];
		int iZec110[32];
		float fRec62_tmp[36];
		float* fRec62 = &fRec62_tmp[4];
		float fZec111[32];
		float fRec68_tmp[36];
		float* fRec68 = &fRec68_tmp[4];
		float fZec112[32];
		float fZec113[32];
		float fZec114[32];
		float fRec70_tmp[36];
		float* fRec70 = &fRec70_tmp[4];
		float fRec69_tmp[36];
		float* fRec69 = &fRec69_tmp[4];
		float fZec115[32];
		float fZec116[32];
		float fZec117[32];
		float fRec71_tmp[36];
		float* fRec71 = &fRec71_tmp[4];
		float fRec61_tmp[36];
		float* fRec61 = &fRec61_tmp[4];
		float fRec60_tmp[36];
		float* fRec60 = &fRec60_tmp[4];
		float fZec118[32];
		float fRec72_tmp[36];
		float* fRec72 = &fRec72_tmp[4];
		float fZec119[32];
		float fZec120[32];
		int iZec121[32];
		float fZec122[32];
		float fRec58_tmp[36];
		float* fRec58 = &fRec58_tmp[4];
		float fRec59[32];
		float fZec123[32];
		float fRec73_tmp[36];
		float* fRec73 = &fRec73_tmp[4];
		float fZec124[32];
		float fZec125[32];
		int iZec126[32];
		float fZec127[32];
		float fRec56_tmp[36];
		float* fRec56 = &fRec56_tmp[4];
		float fRec57[32];
		float fZec128[32];
		float fRec78_tmp[36];
		float* fRec78 = &fRec78_tmp[4];
		float fSlow28 = fConst46 * fSlow13;
		float fZec129[32];
		float fZec130[32];
		float fRec74_tmp[36];
		float* fRec74 = &fRec74_tmp[4];
		float fRec75_tmp[36];
		float* fRec75 = &fRec75_tmp[4];
		float fRec76_tmp[36];
		float* fRec76 = &fRec76_tmp[4];
		float fRec77_tmp[36];
		float* fRec77 = &fRec77_tmp[4];
		float fZec131[32];
		float fZec132[32];
		float fRec81_tmp[36];
		float* fRec81 = &fRec81_tmp[4];
		float fZec133[32];
		float fRec80_tmp[36];
		float* fRec80 = &fRec80_tmp[4];
		float fRec79_tmp[36];
		float* fRec79 = &fRec79_tmp[4];
		float fZec134[32];
		float fZec135[32];
		float fZec136[32];
		float fRec83_tmp[36];
		float* fRec83 = &fRec83_tmp[4];
		float fRec82_tmp[36];
		float* fRec82 = &fRec82_tmp[4];
		float fZec137[32];
		float fZec138[32];
		float fRec84_tmp[36];
		float* fRec84 = &fRec84_tmp[4];
		float fZec139[32];
		int iZec140[32];
		float fZec141[32];
		int vindex = 0;
		/* Main loop */
		for (vindex = 0; vindex <= (count - 32); vindex = vindex + 32) {
			FAUSTFLOAT* input0 = &input0_ptr[vindex];
			FAUSTFLOAT* input1 = &input1_ptr[vindex];
			FAUSTFLOAT* output0 = &output0_ptr[vindex];
			FAUSTFLOAT* output1 = &output1_ptr[vindex];
			int vsize = 32;
			/* Vectorizable loop 0 */
			/* Pre code */
			for (int j10 = 0; j10 < 4; j10 = j10 + 1) {
				iVec0_tmp[j10] = iVec0_perm[j10];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iVec0[i] = 1;
			}
			/* Post code */
			for (int j11 = 0; j11 < 4; j11 = j11 + 1) {
				iVec0_perm[j11] = iVec0_tmp[vsize + j11];
			}
			/* Vectorizable loop 1 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec0[i] = 1 - iVec0[i - 1];
			}
			/* Recursive loop 2 */
			/* Pre code */
			for (int j6 = 0; j6 < 4; j6 = j6 + 1) {
				fRec11_tmp[j6] = fRec11_perm[j6];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec11[i] = fSlow4 + fConst2 * fRec11[i - 1];
			}
			/* Post code */
			for (int j7 = 0; j7 < 4; j7 = j7 + 1) {
				fRec11_perm[j7] = fRec11_tmp[vsize + j7];
			}
			/* Recursive loop 3 */
			/* Pre code */
			for (int j8 = 0; j8 < 4; j8 = j8 + 1) {
				iRec13_tmp[j8] = iRec13_perm[j8];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec13[i] = 1103515245 * iRec13[i - 1] + 12345;
			}
			/* Post code */
			for (int j9 = 0; j9 < 4; j9 = j9 + 1) {
				iRec13_perm[j9] = iRec13_tmp[vsize + j9];
			}
			/* Recursive loop 4 */
			/* Pre code */
			for (int j40 = 0; j40 < 4; j40 = j40 + 1) {
				fRec27_tmp[j40] = fRec27_perm[j40];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec20[i] = ((iZec0[i]) ? 0.0f : fConst16 + fRec27[i - 1]);
				fRec27[i] = fZec20[i] - std::floor(fZec20[i]);
			}
			/* Post code */
			for (int j41 = 0; j41 < 4; j41 = j41 + 1) {
				fRec27_perm[j41] = fRec27_tmp[vsize + j41];
			}
			/* Recursive loop 5 */
			/* Pre code */
			for (int j110 = 0; j110 < 4; j110 = j110 + 1) {
				fRec65_tmp[j110] = fRec65_perm[j110];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec65[i] = fSlow27 + fConst2 * fRec65[i - 1];
			}
			/* Post code */
			for (int j111 = 0; j111 < 4; j111 = j111 + 1) {
				fRec65_perm[j111] = fRec65_tmp[vsize + j111];
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec1[i] = 1.99f * fRec11[i];
			}
			/* Vectorizable loop 7 */
			/* Pre code */
			for (int j14 = 0; j14 < 4; j14 = j14 + 1) {
				fYec0_tmp[j14] = fYec0_perm[j14];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec0[i] = float(iRec13[i]);
			}
			/* Post code */
			for (int j15 = 0; j15 < 4; j15 = j15 + 1) {
				fYec0_perm[j15] = fYec0_tmp[vsize + j15];
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec4[i] = 18.0f * fRec11[i];
			}
			/* Recursive loop 9 */
			/* Pre code */
			for (int j20 = 0; j20 < 4; j20 = j20 + 1) {
				fRec17_tmp[j20] = fRec17_perm[j20];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = ((iZec0[i]) ? 0.0f : fConst5 + fRec17[i - 1]);
				fRec17[i] = fZec6[i] - std::floor(fZec6[i]);
			}
			/* Post code */
			for (int j21 = 0; j21 < 4; j21 = j21 + 1) {
				fRec17_perm[j21] = fRec17_tmp[vsize + j21];
			}
			/* Vectorizable loop 10 */
			/* Pre code */
			for (int j42 = 0; j42 < 4; j42 = j42 + 1) {
				fYec1_tmp[j42] = fYec1_perm[j42];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec1[i] = fRec27[i] - fRec27[i - 1];
			}
			/* Post code */
			for (int j43 = 0; j43 < 4; j43 = j43 + 1) {
				fYec1_perm[j43] = fYec1_tmp[vsize + j43];
			}
			/* Recursive loop 11 */
			/* Pre code */
			for (int j46 = 0; j46 < 4; j46 = j46 + 1) {
				fRec30_tmp[j46] = fRec30_perm[j46];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec22[i] = ((iZec0[i]) ? 0.0f : fConst18 + fRec30[i - 1]);
				fRec30[i] = fZec22[i] - std::floor(fZec22[i]);
			}
			/* Post code */
			for (int j47 = 0; j47 < 4; j47 = j47 + 1) {
				fRec30_perm[j47] = fRec30_tmp[vsize + j47];
			}
			/* Vectorizable loop 12 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec104[i] = 0.2f * fRec65[i];
			}
			/* Recursive loop 13 */
			/* Pre code */
			for (int j12 = 0; j12 < 4; j12 = j12 + 1) {
				fRec14_tmp[j12] = fRec14_perm[j12];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec2[i] = ((iZec0[i]) ? 0.0f : fRec14[i - 1] + fConst4 * (fZec1[i] + 0.01f));
				fRec14[i] = fZec2[i] - std::floor(fZec2[i]);
			}
			/* Post code */
			for (int j13 = 0; j13 < 4; j13 = j13 + 1) {
				fRec14_perm[j13] = fRec14_tmp[vsize + j13];
			}
			/* Vectorizable loop 14 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec3[i] = 4.6566128e-12f * fRec11[i] * fYec0[i];
			}
			/* Recursive loop 15 */
			/* Pre code */
			for (int j18 = 0; j18 < 4; j18 = j18 + 1) {
				fRec16_tmp[j18] = fRec16_perm[j18];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec5[i] = ((iZec0[i]) ? 0.0f : fRec16[i - 1] + fConst4 * (fZec4[i] + 2.0f));
				fRec16[i] = fZec5[i] - std::floor(fZec5[i]);
			}
			/* Post code */
			for (int j19 = 0; j19 < 4; j19 = j19 + 1) {
				fRec16_perm[j19] = fRec16_tmp[vsize + j19];
			}
			/* Vectorizable loop 16 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = 0.2f * ftbl0MacallaSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec17[i]), 65535))] + 0.8f;
			}
			/* Vectorizable loop 17 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec21[i] = (fYec1[i - 1] <= 0.0f) & (fYec1[i] > 0.0f);
			}
			/* Vectorizable loop 18 */
			/* Pre code */
			for (int j48 = 0; j48 < 4; j48 = j48 + 1) {
				fYec2_tmp[j48] = fYec2_perm[j48];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec2[i] = Macalla_faustpower2_f(2.0f * fRec30[i] + -1.0f);
			}
			/* Post code */
			for (int j49 = 0; j49 < 4; j49 = j49 + 1) {
				fYec2_perm[j49] = fYec2_tmp[vsize + j49];
			}
			/* Recursive loop 19 */
			/* Pre code */
			for (int j58 = 0; j58 < 4; j58 = j58 + 1) {
				fRec33_tmp[j58] = fRec33_perm[j58];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec33[i] = fSlow7 + fConst2 * fRec33[i - 1];
			}
			/* Post code */
			for (int j59 = 0; j59 < 4; j59 = j59 + 1) {
				fRec33_perm[j59] = fRec33_tmp[vsize + j59];
			}
			/* Recursive loop 20 */
			/* Pre code */
			for (int j78 = 0; j78 < 4; j78 = j78 + 1) {
				fRec43_tmp[j78] = fRec43_perm[j78];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec43[i] = fSlow12 + fConst2 * fRec43[i - 1];
			}
			/* Post code */
			for (int j79 = 0; j79 < 4; j79 = j79 + 1) {
				fRec43_perm[j79] = fRec43_tmp[vsize + j79];
			}
			/* Recursive loop 21 */
			/* Pre code */
			for (int j112 = 0; j112 < 4; j112 = j112 + 1) {
				fRec64_tmp[j112] = fRec64_perm[j112];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec105[i] = ((iZec0[i]) ? 0.0f : fRec64[i - 1] + fConst4 * (fZec1[i] + fZec104[i] + 0.01f));
				fRec64[i] = fZec105[i] - std::floor(fZec105[i]);
			}
			/* Post code */
			for (int j113 = 0; j113 < 4; j113 = j113 + 1) {
				fRec64_perm[j113] = fRec64_tmp[vsize + j113];
			}
			/* Recursive loop 22 */
			/* Pre code */
			for (int j116 = 0; j116 < 4; j116 = j116 + 1) {
				fRec67_tmp[j116] = fRec67_perm[j116];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec106[i] = ((iZec0[i]) ? 0.0f : fRec67[i - 1] + fConst4 * (fZec4[i] + fZec104[i] + 2.0f));
				fRec67[i] = fZec106[i] - std::floor(fZec106[i]);
			}
			/* Post code */
			for (int j117 = 0; j117 < 4; j117 = j117 + 1) {
				fRec67_perm[j117] = fRec67_tmp[vsize + j117];
			}
			/* Recursive loop 23 */
			/* Pre code */
			for (int j2 = 0; j2 < 4; j2 = j2 + 1) {
				fRec2_tmp[j2] = fRec2_perm[j2];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec2[i] = std::fmod(fRec2[i - 1] + 4097.0f - fSlow2, 4096.0f);
			}
			/* Post code */
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				fRec2_perm[j3] = fRec2_tmp[vsize + j3];
			}
			/* Recursive loop 24 */
			/* Pre code */
			for (int j16 = 0; j16 < 4; j16 = j16 + 1) {
				fRec12_tmp[j16] = fRec12_perm[j16];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec12[i] = 0.0001f * (fZec3[i] + (1.0f - std::fabs(2.0f * fRec14[i] + -1.0f))) + 0.9999f * fRec12[i - 1];
			}
			/* Post code */
			for (int j17 = 0; j17 < 4; j17 = j17 + 1) {
				fRec12_perm[j17] = fRec12_tmp[vsize + j17];
			}
			/* Recursive loop 25 */
			/* Pre code */
			for (int j22 = 0; j22 < 4; j22 = j22 + 1) {
				fRec15_tmp[j22] = fRec15_perm[j22];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec15[i] = 0.0001f * (fZec3[i] + (1.0f - std::fabs(2.0f * fRec16[i] + -1.0f))) * fZec7[i] + 0.9999f * fRec15[i - 1];
			}
			/* Post code */
			for (int j23 = 0; j23 < 4; j23 = j23 + 1) {
				fRec15_perm[j23] = fRec15_tmp[vsize + j23];
			}
			/* Recursive loop 26 */
			/* Pre code */
			for (int j44 = 0; j44 < 4; j44 = j44 + 1) {
				fRec28_tmp[j44] = fRec28_perm[j44];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec28[i] = fRec28[i - 1] * float(1 - iZec21[i]) + 4.656613e-10f * fYec0[i] * float(iZec21[i]);
			}
			/* Post code */
			for (int j45 = 0; j45 < 4; j45 = j45 + 1) {
				fRec28_perm[j45] = fRec28_tmp[vsize + j45];
			}
			/* Vectorizable loop 27 */
			/* Pre code */
			fYec3_idx = (fYec3_idx + fYec3_idx_save) & 2047;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec3[(i + fYec3_idx) & 2047] = float(iVec0[i - 1]) * (fYec2[i] - fYec2[i - 1]);
			}
			/* Post code */
			fYec3_idx_save = vsize;
			/* Vectorizable loop 28 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec28[i] = std::tan(fConst40 * fRec33[i]);
			}
			/* Recursive loop 29 */
			/* Pre code */
			for (int j62 = 0; j62 < 4; j62 = j62 + 1) {
				fRec34_tmp[j62] = fRec34_perm[j62];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec34[i] = fSlow8 + fConst2 * fRec34[i - 1];
			}
			/* Post code */
			for (int j63 = 0; j63 < 4; j63 = j63 + 1) {
				fRec34_perm[j63] = fRec34_tmp[vsize + j63];
			}
			/* Recursive loop 30 */
			/* Pre code */
			for (int j68 = 0; j68 < 4; j68 = j68 + 1) {
				fRec36_tmp[j68] = fRec36_perm[j68];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec40[i] = ((iZec0[i]) ? 0.0f : fConst42 + fRec36[i - 1]);
				fRec36[i] = fZec40[i] - std::floor(fZec40[i]);
			}
			/* Post code */
			for (int j69 = 0; j69 < 4; j69 = j69 + 1) {
				fRec36_perm[j69] = fRec36_tmp[vsize + j69];
			}
			/* Recursive loop 31 */
			/* Pre code */
			for (int j72 = 0; j72 < 4; j72 = j72 + 1) {
				fRec37_tmp[j72] = fRec37_perm[j72];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec45[i] = ((iZec0[i]) ? 0.0f : fConst47 + fRec37[i - 1]);
				fRec37[i] = fZec45[i] - std::floor(fZec45[i]);
			}
			/* Post code */
			for (int j73 = 0; j73 < 4; j73 = j73 + 1) {
				fRec37_perm[j73] = fRec37_tmp[vsize + j73];
			}
			/* Recursive loop 32 */
			/* Pre code */
			for (int j76 = 0; j76 < 4; j76 = j76 + 1) {
				fRec38_tmp[j76] = fRec38_perm[j76];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec38[i] = std::fmod(fRec38[i - 1] + 4097.0f - fSlow11, 4096.0f);
			}
			/* Post code */
			for (int j77 = 0; j77 < 4; j77 = j77 + 1) {
				fRec38_perm[j77] = fRec38_tmp[vsize + j77];
			}
			/* Vectorizable loop 33 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec50[i] = 4.99f * fRec43[i];
			}
			/* Recursive loop 34 */
			/* Pre code */
			for (int j114 = 0; j114 < 4; j114 = j114 + 1) {
				fRec63_tmp[j114] = fRec63_perm[j114];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec63[i] = 0.0001f * (fZec3[i] + (1.0f - std::fabs(2.0f * fRec64[i] + -1.0f))) + 0.9999f * fRec63[i - 1];
			}
			/* Post code */
			for (int j115 = 0; j115 < 4; j115 = j115 + 1) {
				fRec63_perm[j115] = fRec63_tmp[vsize + j115];
			}
			/* Recursive loop 35 */
			/* Pre code */
			for (int j118 = 0; j118 < 4; j118 = j118 + 1) {
				fRec66_tmp[j118] = fRec66_perm[j118];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec66[i] = 0.0001f * fZec7[i] * (fZec3[i] + (1.0f - std::fabs(2.0f * fRec67[i] + -1.0f))) + 0.9999f * fRec66[i - 1];
			}
			/* Post code */
			for (int j119 = 0; j119 < 4; j119 = j119 + 1) {
				fRec66_perm[j119] = fRec66_tmp[vsize + j119];
			}
			/* Recursive loop 36 */
			/* Pre code */
			for (int j134 = 0; j134 < 4; j134 = j134 + 1) {
				fRec72_tmp[j134] = fRec72_perm[j134];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec118[i] = ((iZec0[i]) ? 0.0f : fRec72[i - 1] + fConst4 * (0.02f * fRec65[i] + 0.15f));
				fRec72[i] = fZec118[i] - std::floor(fZec118[i]);
			}
			/* Post code */
			for (int j135 = 0; j135 < 4; j135 = j135 + 1) {
				fRec72_perm[j135] = fRec72_tmp[vsize + j135];
			}
			/* Recursive loop 37 */
			/* Pre code */
			for (int j138 = 0; j138 < 4; j138 = j138 + 1) {
				fRec73_tmp[j138] = fRec73_perm[j138];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec123[i] = ((iZec0[i]) ? 0.0f : fRec73[i - 1] + fConst4 * (0.03f * fRec65[i] + 0.23f));
				fRec73[i] = fZec123[i] - std::floor(fZec123[i]);
			}
			/* Post code */
			for (int j139 = 0; j139 < 4; j139 = j139 + 1) {
				fRec73_perm[j139] = fRec73_tmp[vsize + j139];
			}
			/* Recursive loop 38 */
			/* Pre code */
			for (int j0 = 0; j0 < 4; j0 = j0 + 1) {
				fRec1_tmp[j0] = fRec1_perm[j0];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec1[i] = fSlow0 + fConst2 * fRec1[i - 1];
			}
			/* Post code */
			for (int j1 = 0; j1 < 4; j1 = j1 + 1) {
				fRec1_perm[j1] = fRec1_tmp[vsize + j1];
			}
			/* Vectorizable loop 39 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec8[i] = fRec11[i] * (6e+02f * fRec12[i] + 1e+02f * fRec15[i]);
			}
			/* Recursive loop 40 */
			/* Pre code */
			for (int j38 = 0; j38 < 4; j38 = j38 + 1) {
				fRec26_tmp[j38] = fRec26_perm[j38];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec26[i] = 0.5221894f * fRec26[i - 3] + 4.656613e-10f * fYec0[i] + 2.494956f * fRec26[i - 1] - 2.0172658f * fRec26[i - 2];
			}
			/* Post code */
			for (int j39 = 0; j39 < 4; j39 = j39 + 1) {
				fRec26_perm[j39] = fRec26_tmp[vsize + j39];
			}
			/* Recursive loop 41 */
			/* Pre code */
			for (int j50 = 0; j50 < 4; j50 = j50 + 1) {
				fRec29_tmp[j50] = fRec29_perm[j50];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec29[i] = 0.999f * fRec29[i - 1] + fConst19 * (fYec3[(i + fYec3_idx) & 2047] - fConst22 * fYec3[(i + fYec3_idx - iConst23) & 2047] - fConst24 * fYec3[(i + fYec3_idx - iConst25) & 2047]);
			}
			/* Post code */
			for (int j51 = 0; j51 < 4; j51 = j51 + 1) {
				fRec29_perm[j51] = fRec29_tmp[vsize + j51];
			}
			/* Recursive loop 42 */
			/* Pre code */
			for (int j52 = 0; j52 < 4; j52 = j52 + 1) {
				fRec31_tmp[j52] = fRec31_perm[j52];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec23[i] = ((iZec0[i]) ? 0.0f : fConst26 + fRec31[i - 1]);
				fRec31[i] = fZec23[i] - std::floor(fZec23[i]);
			}
			/* Post code */
			for (int j53 = 0; j53 < 4; j53 = j53 + 1) {
				fRec31_perm[j53] = fRec31_tmp[vsize + j53];
			}
			/* Recursive loop 43 */
			/* Pre code */
			for (int j54 = 0; j54 < 4; j54 = j54 + 1) {
				fRec32_tmp[j54] = fRec32_perm[j54];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec24[i] = ((iZec0[i]) ? 0.0f : fConst27 + fRec32[i - 1]);
				fRec32[i] = fZec24[i] - std::floor(fZec24[i]);
			}
			/* Post code */
			for (int j55 = 0; j55 < 4; j55 = j55 + 1) {
				fRec32_perm[j55] = fRec32_tmp[vsize + j55];
			}
			/* Vectorizable loop 44 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec25[i] = 0.5f * (fRec28[i] + 1.0f);
			}
			/* Vectorizable loop 45 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec29[i] = 1.0f / fZec28[i];
			}
			/* Vectorizable loop 46 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec35[i] = std::tan(fConst40 * fRec34[i]);
			}
			/* Vectorizable loop 47 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec41[i] = std::max<float>(0.0f, std::min<float>(fConst45, fConst46 * (1.5f * (2.0f * (1.0f - std::fabs(2.0f * fRec36[i] + -1.0f)) + -1.0f) + 7.0f)));
			}
			/* Vectorizable loop 48 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec46[i] = std::max<float>(0.0f, std::min<float>(fConst45, fConst46 * (1.5f * (2.0f * (1.0f - std::fabs(2.0f * fRec37[i] + -1.0f)) + -1.0f) + 11.3f)));
			}
			/* Recursive loop 49 */
			/* Pre code */
			for (int j80 = 0; j80 < 4; j80 = j80 + 1) {
				fRec44_tmp[j80] = fRec44_perm[j80];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec51[i] = ((iZec0[i]) ? 0.0f : fRec44[i - 1] + fConst4 * (fZec50[i] + 0.01f));
				fRec44[i] = fZec51[i] - std::floor(fZec51[i]);
			}
			/* Post code */
			for (int j81 = 0; j81 < 4; j81 = j81 + 1) {
				fRec44_perm[j81] = fRec44_tmp[vsize + j81];
			}
			/* Vectorizable loop 50 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec62[i] = fRec2[i] + 4096.0f;
			}
			/* Vectorizable loop 51 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec77[i] = fRec38[i] + 4096.0f;
			}
			/* Vectorizable loop 52 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec107[i] = fRec11[i] * (6e+02f * fRec63[i] + 1e+02f * fRec66[i]);
			}
			/* Vectorizable loop 53 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec119[i] = std::max<float>(0.0f, std::min<float>(fConst45, fConst46 * (0.5f * fRec65[i] + 1.5f * (2.0f * (1.0f - std::fabs(2.0f * fRec72[i] + -1.0f)) + -1.0f) + 7.0f)));
			}
			/* Vectorizable loop 54 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec124[i] = std::max<float>(0.0f, std::min<float>(fConst45, fConst46 * (0.7f * fRec65[i] + 1.5f * (2.0f * (1.0f - std::fabs(2.0f * fRec73[i] + -1.0f)) + -1.0f) + 11.3f)));
			}
			/* Recursive loop 55 */
			/* Pre code */
			for (int j142 = 0; j142 < 4; j142 = j142 + 1) {
				fRec78_tmp[j142] = fRec78_perm[j142];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec128[i] = ((iZec0[i]) ? 0.0f : fRec78[i - 1] + fConst4 * (fZec50[i] + 2.0f * fRec65[i] + 0.01f));
				fRec78[i] = fZec128[i] - std::floor(fZec128[i]);
			}
			/* Post code */
			for (int j143 = 0; j143 < 4; j143 = j143 + 1) {
				fRec78_perm[j143] = fRec78_tmp[vsize + j143];
			}
			/* Vectorizable loop 56 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = fZec8[i] + 100.00001f;
			}
			/* Recursive loop 57 */
			/* Pre code */
			for (int j26 = 0; j26 < 4; j26 = j26 + 1) {
				iRec20_tmp[j26] = iRec20_perm[j26];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec20[i] = iRec20[i - 1] + 1;
			}
			/* Post code */
			for (int j27 = 0; j27 < 4; j27 = j27 + 1) {
				iRec20_perm[j27] = iRec20_tmp[vsize + j27];
			}
			/* Recursive loop 58 */
			/* Pre code */
			for (int j36 = 0; j36 < 4; j36 = j36 + 1) {
				fRec24_tmp[j36] = fRec24_perm[j36];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec24[i] = fSlow6 + fConst2 * fRec24[i - 1];
			}
			/* Post code */
			for (int j37 = 0; j37 < 4; j37 = j37 + 1) {
				fRec24_perm[j37] = fRec24_tmp[vsize + j37];
			}
			/* Recursive loop 59 */
			/* Pre code */
			for (int j56 = 0; j56 < 4; j56 = j56 + 1) {
				fRec25_tmp[j56] = fRec25_perm[j56];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec25[i] = 1.2f * (0.049922034f * fRec26[i] + 0.0506127f * fRec26[i - 2] - (0.095993534f * fRec26[i - 1] + 0.004408786f * fRec26[i - 3])) + 2.3283064e-10f * fYec0[i - 1] * float((fRec27[i] >= fZec25[i]) * (fRec27[i - 1] < fZec25[i])) + fConst28 * fRec29[i] + 0.01f * (ftbl0MacallaSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec31[i]), 65535))] + ftbl0MacallaSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec32[i]), 65535))]) - fConst36 * (fConst37 * fRec25[i - 1] + fConst38 * fRec25[i - 2]);
			}
			/* Post code */
			for (int j57 = 0; j57 < 4; j57 = j57 + 1) {
				fRec25_perm[j57] = fRec25_tmp[vsize + j57];
			}
			/* Vectorizable loop 60 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec31[i] = Macalla_faustpower2_f(fZec28[i]);
			}
			/* Vectorizable loop 61 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec33[i] = (fZec29[i] + 1.4142135f) / fZec28[i] + 1.0f;
			}
			/* Vectorizable loop 62 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec36[i] = 1.0f / fZec35[i];
			}
			/* Vectorizable loop 63 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec42[i] = fZec41[i] + -1.0f;
			}
			/* Vectorizable loop 64 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec47[i] = fZec46[i] + -1.0f;
			}
			/* Vectorizable loop 65 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec52[i] = std::min<float>(524288.0f, fConst0 * (fSlow14 + fRec43[i] * std::fabs(2.0f * (1.0f - std::fabs(2.0f * fRec44[i] + -1.0f)) + -1.0f)));
			}
			/* Vectorizable loop 66 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec54[i] = std::pow(1e+01f, 0.05f * fRec1[i]);
			}
			/* Vectorizable loop 67 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec55[i] = int(fRec2[i]);
			}
			/* Vectorizable loop 68 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec57[i] = std::floor(fRec2[i]);
			}
			/* Vectorizable loop 69 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec61[i] = std::min<float>(0.001953125f * fRec2[i], 1.0f);
			}
			/* Vectorizable loop 70 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec63[i] = int(fZec62[i]);
			}
			/* Vectorizable loop 71 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec65[i] = std::floor(fZec62[i]);
			}
			/* Vectorizable loop 72 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec70[i] = int(fRec38[i]);
			}
			/* Vectorizable loop 73 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec72[i] = std::floor(fRec38[i]);
			}
			/* Vectorizable loop 74 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec76[i] = std::min<float>(0.001953125f * fRec38[i], 1.0f);
			}
			/* Vectorizable loop 75 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec78[i] = int(fZec77[i]);
			}
			/* Vectorizable loop 76 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec80[i] = std::floor(fZec77[i]);
			}
			/* Recursive loop 77 */
			/* Pre code */
			for (int j92 = 0; j92 < 4; j92 = j92 + 1) {
				fRec49_tmp[j92] = fRec49_perm[j92];
			}
			for (int j94 = 0; j94 < 4; j94 = j94 + 1) {
				fRec50_tmp[j94] = fRec50_perm[j94];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec49[i] = fConst48 * fRec50[i - 1] + fConst49 * fRec49[i - 1];
				fRec50[i] = float(iZec0[i]) + fConst49 * fRec50[i - 1] - fConst48 * fRec49[i - 1];
			}
			/* Post code */
			for (int j93 = 0; j93 < 4; j93 = j93 + 1) {
				fRec49_perm[j93] = fRec49_tmp[vsize + j93];
			}
			for (int j95 = 0; j95 < 4; j95 = j95 + 1) {
				fRec50_perm[j95] = fRec50_tmp[vsize + j95];
			}
			/* Vectorizable loop 78 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec108[i] = fZec107[i] + 100.00001f;
			}
			/* Vectorizable loop 79 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec120[i] = fZec119[i] + -1.0f;
			}
			/* Vectorizable loop 80 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec125[i] = fZec124[i] + -1.0f;
			}
			/* Vectorizable loop 81 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec129[i] = std::min<float>(524288.0f, fSlow28 + 4e+01f * fRec65[i] + fRec43[i] * (fConst0 + 1e+03f * fRec65[i]) * std::fabs(2.0f * (1.0f - std::fabs(2.0f * fRec78[i] + -1.0f)) + -1.0f));
			}
			/* Recursive loop 82 */
			/* Pre code */
			for (int j4 = 0; j4 < 4; j4 = j4 + 1) {
				fRec3_tmp[j4] = fRec3_perm[j4];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec3[i] = fSlow3 + fConst2 * fRec3[i - 1];
			}
			/* Post code */
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				fRec3_perm[j5] = fRec3_tmp[vsize + j5];
			}
			/* Vectorizable loop 83 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec10[i] = std::floor(fZec9[i]);
			}
			/* Vectorizable loop 84 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec11[i] = int(fZec9[i]);
			}
			/* Vectorizable loop 85 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec12[i] = (iRec20[i - 1] % iConst11) == 0;
			}
			/* Vectorizable loop 86 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec26[i] = Macalla_faustpower2_f(fRec24[i]);
			}
			/* Vectorizable loop 87 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec27[i] = fRec25[i] - fRec25[i - 2];
			}
			/* Vectorizable loop 88 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec30[i] = (fZec29[i] + -1.4142135f) / fZec28[i] + 1.0f;
			}
			/* Vectorizable loop 89 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec32[i] = 1.0f - 1.0f / fZec31[i];
			}
			/* Vectorizable loop 90 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec37[i] = (fZec36[i] + -1.4142135f) / fZec35[i] + 1.0f;
			}
			/* Vectorizable loop 91 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec34[i] = fZec31[i] * fZec33[i];
			}
			/* Vectorizable loop 92 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec38[i] = 1.0f - 1.0f / Macalla_faustpower2_f(fZec35[i]);
			}
			/* Vectorizable loop 93 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec39[i] = (fZec36[i] + 1.4142135f) / fZec35[i] + 1.0f;
			}
			/* Recursive loop 94 */
			/* Pre code */
			for (int j66 = 0; j66 < 4; j66 = j66 + 1) {
				fRec35_tmp[j66] = fRec35_perm[j66];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec35[i] = fSlow9 + fConst2 * fRec35[i - 1];
			}
			/* Post code */
			for (int j67 = 0; j67 < 4; j67 = j67 + 1) {
				fRec35_perm[j67] = fRec35_tmp[vsize + j67];
			}
			/* Vectorizable loop 95 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec44[i] = std::floor(fZec42[i]);
			}
			/* Vectorizable loop 96 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec43[i] = int(fZec42[i]);
			}
			/* Vectorizable loop 97 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec48[i] = int(fZec47[i]);
			}
			/* Vectorizable loop 98 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec49[i] = std::floor(fZec47[i]);
			}
			/* Recursive loop 99 */
			/* Pre code */
			for (int j82 = 0; j82 < 4; j82 = j82 + 1) {
				fRec39_tmp[j82] = fRec39_perm[j82];
			}
			for (int j84 = 0; j84 < 4; j84 = j84 + 1) {
				fRec40_tmp[j84] = fRec40_perm[j84];
			}
			for (int j86 = 0; j86 < 4; j86 = j86 + 1) {
				fRec41_tmp[j86] = fRec41_perm[j86];
			}
			for (int j88 = 0; j88 < 4; j88 = j88 + 1) {
				fRec42_tmp[j88] = fRec42_perm[j88];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec53[i] = ((fRec39[i - 1] != 0.0f) ? (((fRec40[i - 1] > 0.0f) & (fRec40[i - 1] < 1.0f)) ? fRec39[i - 1] : 0.0f) : (((fRec40[i - 1] == 0.0f) & (fZec52[i] != fRec41[i - 1])) ? fConst16 : (((fRec40[i - 1] == 1.0f) & (fZec52[i] != fRec42[i - 1])) ? -fConst16 : 0.0f)));
				fRec39[i] = fZec53[i];
				fRec40[i] = std::max<float>(0.0f, std::min<float>(1.0f, fRec40[i - 1] + fZec53[i]));
				fRec41[i] = (((fRec40[i - 1] >= 1.0f) & (fRec42[i - 1] != fZec52[i])) ? fZec52[i] : fRec41[i - 1]);
				fRec42[i] = (((fRec40[i - 1] <= 0.0f) & (fRec41[i - 1] != fZec52[i])) ? fZec52[i] : fRec42[i - 1]);
			}
			/* Post code */
			for (int j83 = 0; j83 < 4; j83 = j83 + 1) {
				fRec39_perm[j83] = fRec39_tmp[vsize + j83];
			}
			for (int j85 = 0; j85 < 4; j85 = j85 + 1) {
				fRec40_perm[j85] = fRec40_tmp[vsize + j85];
			}
			for (int j87 = 0; j87 < 4; j87 = j87 + 1) {
				fRec41_perm[j87] = fRec41_tmp[vsize + j87];
			}
			for (int j89 = 0; j89 < 4; j89 = j89 + 1) {
				fRec42_perm[j89] = fRec42_tmp[vsize + j89];
			}
			/* Vectorizable loop 100 */
			/* Pre code */
			fYec6_idx = (fYec6_idx + fYec6_idx_save) & 131071;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec6[(i + fYec6_idx) & 131071] = float(input0[i]) * fZec54[i];
			}
			/* Post code */
			fYec6_idx_save = vsize;
			/* Vectorizable loop 101 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec56[i] = std::min<int>(65537, std::max<int>(0, iZec55[i]));
			}
			/* Vectorizable loop 102 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec58[i] = fZec57[i] + (1.0f - fRec2[i]);
			}
			/* Vectorizable loop 103 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec59[i] = fRec2[i] - fZec57[i];
			}
			/* Vectorizable loop 104 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec60[i] = std::min<int>(65537, std::max<int>(0, iZec55[i] + 1));
			}
			/* Vectorizable loop 105 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec64[i] = std::min<int>(65537, std::max<int>(0, iZec63[i]));
			}
			/* Vectorizable loop 106 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec66[i] = fZec65[i] + (-4095.0f - fRec2[i]);
			}
			/* Vectorizable loop 107 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec67[i] = fRec2[i] + (4096.0f - fZec65[i]);
			}
			/* Vectorizable loop 108 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec68[i] = std::min<int>(65537, std::max<int>(0, iZec63[i] + 1));
			}
			/* Vectorizable loop 109 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec69[i] = 1.0f - fZec61[i];
			}
			/* Vectorizable loop 110 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec71[i] = std::min<int>(65537, std::max<int>(0, iZec70[i]));
			}
			/* Vectorizable loop 111 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec73[i] = fZec72[i] + (1.0f - fRec38[i]);
			}
			/* Vectorizable loop 112 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec74[i] = fRec38[i] - fZec72[i];
			}
			/* Vectorizable loop 113 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec75[i] = std::min<int>(65537, std::max<int>(0, iZec70[i] + 1));
			}
			/* Vectorizable loop 114 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec79[i] = std::min<int>(65537, std::max<int>(0, iZec78[i]));
			}
			/* Vectorizable loop 115 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec81[i] = fZec80[i] + (-4095.0f - fRec38[i]);
			}
			/* Vectorizable loop 116 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec82[i] = fRec38[i] + (4096.0f - fZec80[i]);
			}
			/* Vectorizable loop 117 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec83[i] = std::min<int>(65537, std::max<int>(0, iZec78[i] + 1));
			}
			/* Vectorizable loop 118 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec84[i] = 1.0f - fZec76[i];
			}
			/* Vectorizable loop 119 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec86[i] = 2984.513f * (1.0f - fRec49[i]) + 314.15927f;
			}
			/* Vectorizable loop 120 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec109[i] = std::floor(fZec108[i]);
			}
			/* Vectorizable loop 121 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec110[i] = int(fZec108[i]);
			}
			/* Vectorizable loop 122 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec121[i] = int(fZec120[i]);
			}
			/* Vectorizable loop 123 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec122[i] = std::floor(fZec120[i]);
			}
			/* Vectorizable loop 124 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec126[i] = int(fZec125[i]);
			}
			/* Vectorizable loop 125 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec127[i] = std::floor(fZec125[i]);
			}
			/* Recursive loop 126 */
			/* Pre code */
			for (int j144 = 0; j144 < 4; j144 = j144 + 1) {
				fRec74_tmp[j144] = fRec74_perm[j144];
			}
			for (int j146 = 0; j146 < 4; j146 = j146 + 1) {
				fRec75_tmp[j146] = fRec75_perm[j146];
			}
			for (int j148 = 0; j148 < 4; j148 = j148 + 1) {
				fRec76_tmp[j148] = fRec76_perm[j148];
			}
			for (int j150 = 0; j150 < 4; j150 = j150 + 1) {
				fRec77_tmp[j150] = fRec77_perm[j150];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec130[i] = ((fRec74[i - 1] != 0.0f) ? (((fRec75[i - 1] > 0.0f) & (fRec75[i - 1] < 1.0f)) ? fRec74[i - 1] : 0.0f) : (((fRec75[i - 1] == 0.0f) & (fZec129[i] != fRec76[i - 1])) ? fConst16 : (((fRec75[i - 1] == 1.0f) & (fZec129[i] != fRec77[i - 1])) ? -fConst16 : 0.0f)));
				fRec74[i] = fZec130[i];
				fRec75[i] = std::max<float>(0.0f, std::min<float>(1.0f, fRec75[i - 1] + fZec130[i]));
				fRec76[i] = (((fRec75[i - 1] >= 1.0f) & (fRec77[i - 1] != fZec129[i])) ? fZec129[i] : fRec76[i - 1]);
				fRec77[i] = (((fRec75[i - 1] <= 0.0f) & (fRec76[i - 1] != fZec129[i])) ? fZec129[i] : fRec77[i - 1]);
			}
			/* Post code */
			for (int j145 = 0; j145 < 4; j145 = j145 + 1) {
				fRec74_perm[j145] = fRec74_tmp[vsize + j145];
			}
			for (int j147 = 0; j147 < 4; j147 = j147 + 1) {
				fRec75_perm[j147] = fRec75_tmp[vsize + j147];
			}
			for (int j149 = 0; j149 < 4; j149 = j149 + 1) {
				fRec76_perm[j149] = fRec76_tmp[vsize + j149];
			}
			for (int j151 = 0; j151 < 4; j151 = j151 + 1) {
				fRec77_perm[j151] = fRec77_tmp[vsize + j151];
			}
			/* Vectorizable loop 127 */
			/* Pre code */
			fYec10_idx = (fYec10_idx + fYec10_idx_save) & 131071;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec10[(i + fYec10_idx) & 131071] = float(input1[i]) * fZec54[i];
			}
			/* Post code */
			fYec10_idx_save = vsize;
			/* Recursive loop 128 */
			/* Pre code */
			for (int j24 = 0; j24 < 4; j24 = j24 + 1) {
				fRec10_tmp[j24] = fRec10_perm[j24];
			}
			for (int j28 = 0; j28 < 4; j28 = j28 + 1) {
				fRec19_tmp[j28] = fRec19_perm[j28];
			}
			for (int j30 = 0; j30 < 4; j30 = j30 + 1) {
				fRec22_tmp[j30] = fRec22_perm[j30];
			}
			for (int j32 = 0; j32 < 4; j32 = j32 + 1) {
				fRec21_tmp[j32] = fRec21_perm[j32];
			}
			for (int j34 = 0; j34 < 4; j34 = j34 + 1) {
				fRec23_tmp[j34] = fRec23_perm[j34];
			}
			for (int j60 = 0; j60 < 4; j60 = j60 + 1) {
				fRec9_tmp[j60] = fRec9_perm[j60];
			}
			for (int j64 = 0; j64 < 4; j64 = j64 + 1) {
				fRec8_tmp[j64] = fRec8_perm[j64];
			}
			fYec4_idx = (fYec4_idx + fYec4_idx_save) & 2047;
			for (int j70 = 0; j70 < 4; j70 = j70 + 1) {
				fRec6_tmp[j70] = fRec6_perm[j70];
			}
			fYec5_idx = (fYec5_idx + fYec5_idx_save) & 4095;
			for (int j74 = 0; j74 < 4; j74 = j74 + 1) {
				fRec4_tmp[j74] = fRec4_perm[j74];
			}
			fYec7_idx = (fYec7_idx + fYec7_idx_save) & 1048575;
			fRec0_idx = (fRec0_idx + fRec0_idx_save) & 262143;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec10[i] = (fZec8[i] + (1e+02f - fZec10[i])) * fRec0[(i + fRec0_idx - (int(std::min<float>(fConst0, float(std::max<int>(0, iZec11[i] + 1)))) + 1)) & 262143] - (fRec0[(i + fRec0_idx - (int(std::min<float>(fConst0, float(std::max<int>(0, iZec11[i])))) + 1)) & 262143] * (fZec8[i] + (99.0f - fZec10[i])) + fConst8 * (fConst9 * fRec10[i - 2] + fConst10 * fRec10[i - 1]));
				fZec13[i] = fConst8 * (fRec10[i - 2] + fRec10[i] + 2.0f * fRec10[i - 1]);
				fRec19[i] = ((iZec12[i]) ? 0.003921569f * std::floor(255.0f * std::fabs(fZec13[i]) + 0.5f) * (float(2 * (fZec13[i] > 0.0f)) + -1.0f) : fRec19[i - 1]);
				fZec14[i] = ((iSlow5) ? fRec19[i] : fZec13[i]);
				fZec15[i] = std::fabs(fZec14[i]);
				fZec16[i] = ((fZec15[i] > fRec22[i - 1]) ? fConst13 : fConst12);
				fRec22[i] = fZec15[i] * (1.0f - fZec16[i]) + fRec22[i - 1] * fZec16[i];
				fRec21[i] = fConst14 * fRec21[i - 1] - fConst15 * std::max<float>(2e+01f * std::log10(std::max<float>(1.1754944e-38f, fRec22[i])) + 6.0f, 0.0f);
				fZec17[i] = fZec14[i] * std::pow(1e+01f, 0.05f * fRec21[i]);
				fZec18[i] = std::fabs(fZec17[i]);
				fZec19[i] = ((fZec18[i] > fRec23[i - 1]) ? fConst17 : fConst12);
				fRec23[i] = fZec18[i] * (1.0f - fZec19[i]) + fRec23[i - 1] * fZec19[i];
				fRec9[i] = fZec17[i] + fConst39 * fRec23[i] * fZec26[i] * fZec27[i] - (fRec9[i - 2] * fZec30[i] + 2.0f * fRec9[i - 1] * fZec32[i]) / fZec33[i];
				fRec8[i] = (fRec9[i - 2] + (fRec9[i] - 2.0f * fRec9[i - 1])) / fZec34[i] - (fRec8[i - 2] * fZec37[i] + 2.0f * fRec8[i - 1] * fZec38[i]) / fZec39[i];
				fYec4[(i + fYec4_idx) & 2047] = 0.98f * ((fRec8[i - 2] + fRec8[i] + 2.0f * fRec8[i - 1]) / fZec39[i]) - fRec35[i] * fRec6[i - 1];
				fRec6[i] = fYec4[(i + fYec4_idx - std::min<int>(iConst44, std::max<int>(0, iZec43[i]))) & 2047] * (fZec44[i] + (2.0f - fZec41[i])) + (fZec41[i] + (-1.0f - fZec44[i])) * fYec4[(i + fYec4_idx - std::min<int>(iConst44, std::max<int>(0, iZec43[i] + 1))) & 2047];
				fRec7[i] = fRec35[i] * fYec4[(i + fYec4_idx) & 2047];
				fYec5[(i + fYec5_idx) & 4095] = fRec7[i] + fRec6[i - 1] - 0.9f * fRec35[i] * fRec4[i - 1];
				fRec4[i] = fYec5[(i + fYec5_idx - std::min<int>(iConst44, std::max<int>(0, iZec48[i]))) & 4095] * (fZec49[i] + (2.0f - fZec46[i])) + (fZec46[i] + (-1.0f - fZec49[i])) * fYec5[(i + fYec5_idx - std::min<int>(iConst44, std::max<int>(0, iZec48[i] + 1))) & 4095];
				fRec5[i] = 0.9f * fRec35[i] * fYec5[(i + fYec5_idx) & 4095];
				fYec7[(i + fYec7_idx) & 1048575] = ((iSlow15) ? (fYec6[(i + fYec6_idx - iZec56[i]) & 131071] * fZec58[i] + fZec59[i] * fYec6[(i + fYec6_idx - iZec60[i]) & 131071]) * fZec61[i] + (fYec6[(i + fYec6_idx - iZec64[i]) & 131071] * fZec66[i] + fZec67[i] * fYec6[(i + fYec6_idx - iZec68[i]) & 131071]) * fZec69[i] : fYec6[(i + fYec6_idx) & 131071]) + fRec3[i] * (fRec5[i] + fRec4[i - 1]) + ((iSlow16) ? (fYec6[(i + fYec6_idx - iZec71[i]) & 131071] * fZec73[i] + fZec74[i] * fYec6[(i + fYec6_idx - iZec75[i]) & 131071]) * fZec76[i] + (fYec6[(i + fYec6_idx - iZec79[i]) & 131071] * fZec81[i] + fZec82[i] * fYec6[(i + fYec6_idx - iZec83[i]) & 131071]) * fZec84[i] : fYec6[(i + fYec6_idx) & 131071]);
				fZec85[i] = fYec7[(i + fYec7_idx - int(std::min<float>(524288.0f, std::max<float>(0.0f, fRec41[i])))) & 1048575];
				fRec0[(i + fRec0_idx) & 262143] = fZec85[i] + fRec40[i] * (fYec7[(i + fYec7_idx - int(std::min<float>(524288.0f, std::max<float>(0.0f, fRec42[i])))) & 1048575] - fZec85[i]);
			}
			/* Post code */
			fYec7_idx_save = vsize;
			fYec5_idx_save = vsize;
			fYec4_idx_save = vsize;
			for (int j35 = 0; j35 < 4; j35 = j35 + 1) {
				fRec23_perm[j35] = fRec23_tmp[vsize + j35];
			}
			for (int j31 = 0; j31 < 4; j31 = j31 + 1) {
				fRec22_perm[j31] = fRec22_tmp[vsize + j31];
			}
			for (int j33 = 0; j33 < 4; j33 = j33 + 1) {
				fRec21_perm[j33] = fRec21_tmp[vsize + j33];
			}
			for (int j29 = 0; j29 < 4; j29 = j29 + 1) {
				fRec19_perm[j29] = fRec19_tmp[vsize + j29];
			}
			for (int j25 = 0; j25 < 4; j25 = j25 + 1) {
				fRec10_perm[j25] = fRec10_tmp[vsize + j25];
			}
			for (int j61 = 0; j61 < 4; j61 = j61 + 1) {
				fRec9_perm[j61] = fRec9_tmp[vsize + j61];
			}
			for (int j65 = 0; j65 < 4; j65 = j65 + 1) {
				fRec8_perm[j65] = fRec8_tmp[vsize + j65];
			}
			for (int j71 = 0; j71 < 4; j71 = j71 + 1) {
				fRec6_perm[j71] = fRec6_tmp[vsize + j71];
			}
			for (int j75 = 0; j75 < 4; j75 = j75 + 1) {
				fRec4_perm[j75] = fRec4_tmp[vsize + j75];
			}
			fRec0_idx_save = vsize;
			/* Recursive loop 129 */
			/* Pre code */
			for (int j90 = 0; j90 < 4; j90 = j90 + 1) {
				fRec45_tmp[j90] = fRec45_perm[j90];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec45[i] = fSlow17 + fConst2 * fRec45[i - 1];
			}
			/* Post code */
			for (int j91 = 0; j91 < 4; j91 = j91 + 1) {
				fRec45_perm[j91] = fRec45_tmp[vsize + j91];
			}
			/* Vectorizable loop 130 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec87[i] = std::cos(fConst52 * fZec86[i]);
			}
			/* Vectorizable loop 131 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec89[i] = std::cos(fConst54 * fZec86[i]);
			}
			/* Recursive loop 132 */
			/* Pre code */
			for (int j120 = 0; j120 < 4; j120 = j120 + 1) {
				fRec62_tmp[j120] = fRec62_perm[j120];
			}
			for (int j122 = 0; j122 < 4; j122 = j122 + 1) {
				fRec68_tmp[j122] = fRec68_perm[j122];
			}
			for (int j124 = 0; j124 < 4; j124 = j124 + 1) {
				fRec70_tmp[j124] = fRec70_perm[j124];
			}
			for (int j126 = 0; j126 < 4; j126 = j126 + 1) {
				fRec69_tmp[j126] = fRec69_perm[j126];
			}
			for (int j128 = 0; j128 < 4; j128 = j128 + 1) {
				fRec71_tmp[j128] = fRec71_perm[j128];
			}
			for (int j130 = 0; j130 < 4; j130 = j130 + 1) {
				fRec61_tmp[j130] = fRec61_perm[j130];
			}
			for (int j132 = 0; j132 < 4; j132 = j132 + 1) {
				fRec60_tmp[j132] = fRec60_perm[j132];
			}
			fYec8_idx = (fYec8_idx + fYec8_idx_save) & 16383;
			for (int j136 = 0; j136 < 4; j136 = j136 + 1) {
				fRec58_tmp[j136] = fRec58_perm[j136];
			}
			fYec9_idx = (fYec9_idx + fYec9_idx_save) & 16383;
			for (int j140 = 0; j140 < 4; j140 = j140 + 1) {
				fRec56_tmp[j140] = fRec56_perm[j140];
			}
			fYec11_idx = (fYec11_idx + fYec11_idx_save) & 1048575;
			fRec55_idx = (fRec55_idx + fRec55_idx_save) & 262143;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec62[i] = (fZec107[i] + (1e+02f - fZec109[i])) * fRec55[(i + fRec55_idx - (int(std::min<float>(fConst0, float(std::max<int>(0, iZec110[i] + 1)))) + 1)) & 262143] - (fRec55[(i + fRec55_idx - (int(std::min<float>(fConst0, float(std::max<int>(0, iZec110[i])))) + 1)) & 262143] * (fZec107[i] + (99.0f - fZec109[i])) + fConst8 * (fConst9 * fRec62[i - 2] + fConst10 * fRec62[i - 1]));
				fZec111[i] = fConst8 * (fRec62[i - 2] + fRec62[i] + 2.0f * fRec62[i - 1]);
				fRec68[i] = ((iZec12[i]) ? 0.003921569f * std::floor(255.0f * std::fabs(fZec111[i]) + 0.5f) * (float(2 * (fZec111[i] > 0.0f)) + -1.0f) : fRec68[i - 1]);
				fZec112[i] = ((iSlow5) ? fRec68[i] : fZec111[i]);
				fZec113[i] = std::fabs(fZec112[i]);
				fZec114[i] = ((fZec113[i] > fRec70[i - 1]) ? fConst13 : fConst12);
				fRec70[i] = fZec113[i] * (1.0f - fZec114[i]) + fRec70[i - 1] * fZec114[i];
				fRec69[i] = fConst14 * fRec69[i - 1] - fConst15 * std::max<float>(2e+01f * std::log10(std::max<float>(1.1754944e-38f, fRec70[i])) + 6.0f, 0.0f);
				fZec115[i] = fZec112[i] * std::pow(1e+01f, 0.05f * fRec69[i]);
				fZec116[i] = std::fabs(fZec115[i]);
				fZec117[i] = ((fZec116[i] > fRec71[i - 1]) ? fConst17 : fConst12);
				fRec71[i] = fZec116[i] * (1.0f - fZec117[i]) + fRec71[i - 1] * fZec117[i];
				fRec61[i] = fZec115[i] + fConst39 * fZec26[i] * fRec71[i] * fZec27[i] - (fZec30[i] * fRec61[i - 2] + 2.0f * fZec32[i] * fRec61[i - 1]) / fZec33[i];
				fRec60[i] = (fRec61[i - 2] + (fRec61[i] - 2.0f * fRec61[i - 1])) / fZec34[i] - (fZec37[i] * fRec60[i - 2] + 2.0f * fZec38[i] * fRec60[i - 1]) / fZec39[i];
				fYec8[(i + fYec8_idx) & 16383] = 0.98f * ((fRec60[i - 2] + fRec60[i] + 2.0f * fRec60[i - 1]) / fZec39[i]) - fRec35[i] * fRec58[i - 1];
				fRec58[i] = fYec8[(i + fYec8_idx - std::min<int>(iConst44, std::max<int>(0, iZec121[i]))) & 16383] * (fZec122[i] + (2.0f - fZec119[i])) + (fZec119[i] + (-1.0f - fZec122[i])) * fYec8[(i + fYec8_idx - std::min<int>(iConst44, std::max<int>(0, iZec121[i] + 1))) & 16383];
				fRec59[i] = fRec35[i] * fYec8[(i + fYec8_idx) & 16383];
				fYec9[(i + fYec9_idx) & 16383] = fRec59[i] + fRec58[i - 1] - 0.9f * fRec35[i] * fRec56[i - 1];
				fRec56[i] = fYec9[(i + fYec9_idx - std::min<int>(iConst44, std::max<int>(0, iZec126[i]))) & 16383] * (fZec127[i] + (2.0f - fZec124[i])) + (fZec124[i] + (-1.0f - fZec127[i])) * fYec9[(i + fYec9_idx - std::min<int>(iConst44, std::max<int>(0, iZec126[i] + 1))) & 16383];
				fRec57[i] = 0.9f * fRec35[i] * fYec9[(i + fYec9_idx) & 16383];
				fYec11[(i + fYec11_idx) & 1048575] = ((iSlow15) ? fZec61[i] * (fZec58[i] * fYec10[(i + fYec10_idx - iZec56[i]) & 131071] + fZec59[i] * fYec10[(i + fYec10_idx - iZec60[i]) & 131071]) + fZec69[i] * (fZec66[i] * fYec10[(i + fYec10_idx - iZec64[i]) & 131071] + fZec67[i] * fYec10[(i + fYec10_idx - iZec68[i]) & 131071]) : fYec10[(i + fYec10_idx) & 131071]) + fRec3[i] * (fRec57[i] + fRec56[i - 1]) + ((iSlow16) ? fZec76[i] * (fZec73[i] * fYec10[(i + fYec10_idx - iZec71[i]) & 131071] + fZec74[i] * fYec10[(i + fYec10_idx - iZec75[i]) & 131071]) + fZec84[i] * (fZec81[i] * fYec10[(i + fYec10_idx - iZec79[i]) & 131071] + fZec82[i] * fYec10[(i + fYec10_idx - iZec83[i]) & 131071]) : fYec10[(i + fYec10_idx) & 131071]);
				fZec131[i] = fYec11[(i + fYec11_idx - int(std::min<float>(524288.0f, std::max<float>(0.0f, fRec76[i])))) & 1048575];
				fRec55[(i + fRec55_idx) & 262143] = fZec131[i] + fRec75[i] * (fYec11[(i + fYec11_idx - int(std::min<float>(524288.0f, std::max<float>(0.0f, fRec77[i])))) & 1048575] - fZec131[i]);
			}
			/* Post code */
			fYec11_idx_save = vsize;
			fYec9_idx_save = vsize;
			fYec8_idx_save = vsize;
			for (int j129 = 0; j129 < 4; j129 = j129 + 1) {
				fRec71_perm[j129] = fRec71_tmp[vsize + j129];
			}
			for (int j125 = 0; j125 < 4; j125 = j125 + 1) {
				fRec70_perm[j125] = fRec70_tmp[vsize + j125];
			}
			for (int j127 = 0; j127 < 4; j127 = j127 + 1) {
				fRec69_perm[j127] = fRec69_tmp[vsize + j127];
			}
			for (int j123 = 0; j123 < 4; j123 = j123 + 1) {
				fRec68_perm[j123] = fRec68_tmp[vsize + j123];
			}
			for (int j121 = 0; j121 < 4; j121 = j121 + 1) {
				fRec62_perm[j121] = fRec62_tmp[vsize + j121];
			}
			for (int j131 = 0; j131 < 4; j131 = j131 + 1) {
				fRec61_perm[j131] = fRec61_tmp[vsize + j131];
			}
			for (int j133 = 0; j133 < 4; j133 = j133 + 1) {
				fRec60_perm[j133] = fRec60_tmp[vsize + j133];
			}
			for (int j137 = 0; j137 < 4; j137 = j137 + 1) {
				fRec58_perm[j137] = fRec58_tmp[vsize + j137];
			}
			for (int j141 = 0; j141 < 4; j141 = j141 + 1) {
				fRec56_perm[j141] = fRec56_tmp[vsize + j141];
			}
			fRec55_idx_save = vsize;
			/* Recursive loop 133 */
			/* Pre code */
			for (int j96 = 0; j96 < 4; j96 = j96 + 1) {
				fRec48_tmp[j96] = fRec48_perm[j96];
			}
			for (int j98 = 0; j98 < 4; j98 = j98 + 1) {
				fRec47_tmp[j98] = fRec47_perm[j98];
			}
			for (int j100 = 0; j100 < 4; j100 = j100 + 1) {
				fRec46_tmp[j100] = fRec46_perm[j100];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec88[i] = fRec48[i - 1] * fZec87[i];
				fRec48[i] = fRec0[(i + fRec0_idx) & 262143] + 0.23f * fRec46[i - 1] + fConst51 * fZec88[i] - fConst53 * fRec48[i - 2];
				fZec90[i] = fRec47[i - 1] * fZec89[i];
				fRec47[i] = fRec48[i - 2] + fConst53 * (fRec48[i] - fRec47[i - 2]) - fConst51 * (fZec88[i] - fZec90[i]);
				fRec46[i] = fRec47[i - 2] + fConst53 * fRec47[i] - fConst51 * fZec90[i];
			}
			/* Post code */
			for (int j97 = 0; j97 < 4; j97 = j97 + 1) {
				fRec48_perm[j97] = fRec48_tmp[vsize + j97];
			}
			for (int j99 = 0; j99 < 4; j99 = j99 + 1) {
				fRec47_perm[j99] = fRec47_tmp[vsize + j99];
			}
			for (int j101 = 0; j101 < 4; j101 = j101 + 1) {
				fRec46_perm[j101] = fRec46_tmp[vsize + j101];
			}
			/* Vectorizable loop 134 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec91[i] = 1.0f - 0.5f * fRec45[i];
			}
			/* Recursive loop 135 */
			/* Pre code */
			for (int j152 = 0; j152 < 4; j152 = j152 + 1) {
				fRec81_tmp[j152] = fRec81_perm[j152];
			}
			for (int j154 = 0; j154 < 4; j154 = j154 + 1) {
				fRec80_tmp[j154] = fRec80_perm[j154];
			}
			for (int j156 = 0; j156 < 4; j156 = j156 + 1) {
				fRec79_tmp[j156] = fRec79_perm[j156];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec132[i] = fZec87[i] * fRec81[i - 1];
				fRec81[i] = fRec55[(i + fRec55_idx) & 262143] + 0.23f * fRec79[i - 1] + fConst51 * fZec132[i] - fConst53 * fRec81[i - 2];
				fZec133[i] = fZec89[i] * fRec80[i - 1];
				fRec80[i] = fRec81[i - 2] + fConst53 * (fRec81[i] - fRec80[i - 2]) - fConst51 * (fZec132[i] - fZec133[i]);
				fRec79[i] = fRec80[i - 2] + fConst53 * fRec80[i] - fConst51 * fZec133[i];
			}
			/* Post code */
			for (int j153 = 0; j153 < 4; j153 = j153 + 1) {
				fRec81_perm[j153] = fRec81_tmp[vsize + j153];
			}
			for (int j155 = 0; j155 < 4; j155 = j155 + 1) {
				fRec80_perm[j155] = fRec80_tmp[vsize + j155];
			}
			for (int j157 = 0; j157 < 4; j157 = j157 + 1) {
				fRec79_perm[j157] = fRec79_tmp[vsize + j157];
			}
			/* Vectorizable loop 136 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec92[i] = fRec0[(i + fRec0_idx) & 262143] * fZec91[i] + 0.5f * fRec45[i] * fRec46[i];
			}
			/* Vectorizable loop 137 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec95[i] = std::fabs(float(input0[i]));
			}
			/* Recursive loop 138 */
			/* Pre code */
			for (int j108 = 0; j108 < 4; j108 = j108 + 1) {
				fRec54_tmp[j108] = fRec54_perm[j108];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec54[i] = fSlow24 + fConst2 * fRec54[i - 1];
			}
			/* Post code */
			for (int j109 = 0; j109 < 4; j109 = j109 + 1) {
				fRec54_perm[j109] = fRec54_tmp[vsize + j109];
			}
			/* Vectorizable loop 139 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec134[i] = fRec55[(i + fRec55_idx) & 262143] * fZec91[i] + 0.5f * fRec45[i] * fRec79[i];
			}
			/* Vectorizable loop 140 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec137[i] = std::fabs(float(input1[i]));
			}
			/* Vectorizable loop 141 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec93[i] = std::fabs(fZec92[i]);
			}
			/* Recursive loop 142 */
			/* Pre code */
			for (int j106 = 0; j106 < 4; j106 = j106 + 1) {
				fRec53_tmp[j106] = fRec53_perm[j106];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec96[i] = ((fZec95[i] > fRec53[i - 1]) ? fSlow23 : fSlow20);
				fRec53[i] = fZec95[i] * (1.0f - fZec96[i]) + fRec53[i - 1] * fZec96[i];
			}
			/* Post code */
			for (int j107 = 0; j107 < 4; j107 = j107 + 1) {
				fRec53_perm[j107] = fRec53_tmp[vsize + j107];
			}
			/* Vectorizable loop 143 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec98[i] = 5e+01f * fRec54[i];
			}
			/* Vectorizable loop 144 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec135[i] = std::fabs(fZec134[i]);
			}
			/* Recursive loop 145 */
			/* Pre code */
			for (int j162 = 0; j162 < 4; j162 = j162 + 1) {
				fRec84_tmp[j162] = fRec84_perm[j162];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec138[i] = ((fZec137[i] > fRec84[i - 1]) ? fSlow23 : fSlow20);
				fRec84[i] = fZec137[i] * (1.0f - fZec138[i]) + fRec84[i - 1] * fZec138[i];
			}
			/* Post code */
			for (int j163 = 0; j163 < 4; j163 = j163 + 1) {
				fRec84_perm[j163] = fRec84_tmp[vsize + j163];
			}
			/* Recursive loop 146 */
			/* Pre code */
			for (int j102 = 0; j102 < 4; j102 = j102 + 1) {
				fRec52_tmp[j102] = fRec52_perm[j102];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec94[i] = ((fZec93[i] > fRec52[i - 1]) ? fConst13 : fConst12);
				fRec52[i] = fZec93[i] * (1.0f - fZec94[i]) + fRec52[i - 1] * fZec94[i];
			}
			/* Post code */
			for (int j103 = 0; j103 < 4; j103 = j103 + 1) {
				fRec52_perm[j103] = fRec52_tmp[vsize + j103];
			}
			/* Vectorizable loop 147 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec97[i] = 2e+01f * std::log10(std::max<float>(1.1754944e-38f, fRec53[i]));
			}
			/* Vectorizable loop 148 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec99[i] = -1.5f - fZec98[i];
			}
			/* Vectorizable loop 149 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec100[i] = 1.5f - fZec98[i];
			}
			/* Recursive loop 150 */
			/* Pre code */
			for (int j158 = 0; j158 < 4; j158 = j158 + 1) {
				fRec83_tmp[j158] = fRec83_perm[j158];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec136[i] = ((fZec135[i] > fRec83[i - 1]) ? fConst13 : fConst12);
				fRec83[i] = fZec135[i] * (1.0f - fZec136[i]) + fRec83[i - 1] * fZec136[i];
			}
			/* Post code */
			for (int j159 = 0; j159 < 4; j159 = j159 + 1) {
				fRec83_perm[j159] = fRec83_tmp[vsize + j159];
			}
			/* Vectorizable loop 151 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec139[i] = 2e+01f * std::log10(std::max<float>(1.1754944e-38f, fRec84[i]));
			}
			/* Recursive loop 152 */
			/* Pre code */
			for (int j104 = 0; j104 < 4; j104 = j104 + 1) {
				fRec51_tmp[j104] = fRec51_perm[j104];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec51[i] = fConst14 * fRec51[i - 1] - fConst15 * std::max<float>(2e+01f * std::log10(std::max<float>(1.1754944e-38f, fRec52[i])) + 6.0f, 0.0f);
			}
			/* Post code */
			for (int j105 = 0; j105 < 4; j105 = j105 + 1) {
				fRec51_perm[j105] = fRec51_tmp[vsize + j105];
			}
			/* Vectorizable loop 153 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec101[i] = (fZec97[i] > fZec99[i]) + (fZec97[i] > fZec100[i]);
			}
			/* Vectorizable loop 154 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec102[i] = fZec97[i] + fZec98[i];
			}
			/* Vectorizable loop 155 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec103[i] = 1.0f - 1.0f / (19.0f * fRec54[i] + 1.0f);
			}
			/* Recursive loop 156 */
			/* Pre code */
			for (int j160 = 0; j160 < 4; j160 = j160 + 1) {
				fRec82_tmp[j160] = fRec82_perm[j160];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec82[i] = fConst14 * fRec82[i - 1] - fConst15 * std::max<float>(2e+01f * std::log10(std::max<float>(1.1754944e-38f, fRec83[i])) + 6.0f, 0.0f);
			}
			/* Post code */
			for (int j161 = 0; j161 < 4; j161 = j161 + 1) {
				fRec82_perm[j161] = fRec82_tmp[vsize + j161];
			}
			/* Vectorizable loop 157 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec140[i] = (fZec139[i] > fZec99[i]) + (fZec139[i] > fZec100[i]);
			}
			/* Vectorizable loop 158 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec141[i] = fZec98[i] + fZec139[i];
			}
			/* Vectorizable loop 159 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(fSlow26 * float(input0[i]) + fSlow25 * fZec92[i] * std::pow(1e+01f, 0.05f * fRec51[i]) * std::pow(1e+01f, -(0.05f * std::max<float>(0.0f, ((iZec101[i] == 0) ? 0.0f : ((iZec101[i] == 1) ? 0.16666667f * Macalla_faustpower2_f(fZec102[i] + 1.5f) : fZec102[i]))) * fZec103[i])));
			}
			/* Vectorizable loop 160 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(fSlow26 * float(input1[i]) + fSlow25 * fZec134[i] * std::pow(1e+01f, 0.05f * fRec82[i]) * std::pow(1e+01f, -(0.05f * fZec103[i] * std::max<float>(0.0f, ((iZec140[i] == 0) ? 0.0f : ((iZec140[i] == 1) ? 0.16666667f * Macalla_faustpower2_f(fZec141[i] + 1.5f) : fZec141[i]))))));
			}
		}
		/* Remaining frames */
		if (vindex < count) {
			FAUSTFLOAT* input0 = &input0_ptr[vindex];
			FAUSTFLOAT* input1 = &input1_ptr[vindex];
			FAUSTFLOAT* output0 = &output0_ptr[vindex];
			FAUSTFLOAT* output1 = &output1_ptr[vindex];
			int vsize = count - vindex;
			/* Vectorizable loop 0 */
			/* Pre code */
			for (int j10 = 0; j10 < 4; j10 = j10 + 1) {
				iVec0_tmp[j10] = iVec0_perm[j10];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iVec0[i] = 1;
			}
			/* Post code */
			for (int j11 = 0; j11 < 4; j11 = j11 + 1) {
				iVec0_perm[j11] = iVec0_tmp[vsize + j11];
			}
			/* Vectorizable loop 1 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec0[i] = 1 - iVec0[i - 1];
			}
			/* Recursive loop 2 */
			/* Pre code */
			for (int j6 = 0; j6 < 4; j6 = j6 + 1) {
				fRec11_tmp[j6] = fRec11_perm[j6];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec11[i] = fSlow4 + fConst2 * fRec11[i - 1];
			}
			/* Post code */
			for (int j7 = 0; j7 < 4; j7 = j7 + 1) {
				fRec11_perm[j7] = fRec11_tmp[vsize + j7];
			}
			/* Recursive loop 3 */
			/* Pre code */
			for (int j8 = 0; j8 < 4; j8 = j8 + 1) {
				iRec13_tmp[j8] = iRec13_perm[j8];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec13[i] = 1103515245 * iRec13[i - 1] + 12345;
			}
			/* Post code */
			for (int j9 = 0; j9 < 4; j9 = j9 + 1) {
				iRec13_perm[j9] = iRec13_tmp[vsize + j9];
			}
			/* Recursive loop 4 */
			/* Pre code */
			for (int j40 = 0; j40 < 4; j40 = j40 + 1) {
				fRec27_tmp[j40] = fRec27_perm[j40];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec20[i] = ((iZec0[i]) ? 0.0f : fConst16 + fRec27[i - 1]);
				fRec27[i] = fZec20[i] - std::floor(fZec20[i]);
			}
			/* Post code */
			for (int j41 = 0; j41 < 4; j41 = j41 + 1) {
				fRec27_perm[j41] = fRec27_tmp[vsize + j41];
			}
			/* Recursive loop 5 */
			/* Pre code */
			for (int j110 = 0; j110 < 4; j110 = j110 + 1) {
				fRec65_tmp[j110] = fRec65_perm[j110];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec65[i] = fSlow27 + fConst2 * fRec65[i - 1];
			}
			/* Post code */
			for (int j111 = 0; j111 < 4; j111 = j111 + 1) {
				fRec65_perm[j111] = fRec65_tmp[vsize + j111];
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec1[i] = 1.99f * fRec11[i];
			}
			/* Vectorizable loop 7 */
			/* Pre code */
			for (int j14 = 0; j14 < 4; j14 = j14 + 1) {
				fYec0_tmp[j14] = fYec0_perm[j14];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec0[i] = float(iRec13[i]);
			}
			/* Post code */
			for (int j15 = 0; j15 < 4; j15 = j15 + 1) {
				fYec0_perm[j15] = fYec0_tmp[vsize + j15];
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec4[i] = 18.0f * fRec11[i];
			}
			/* Recursive loop 9 */
			/* Pre code */
			for (int j20 = 0; j20 < 4; j20 = j20 + 1) {
				fRec17_tmp[j20] = fRec17_perm[j20];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = ((iZec0[i]) ? 0.0f : fConst5 + fRec17[i - 1]);
				fRec17[i] = fZec6[i] - std::floor(fZec6[i]);
			}
			/* Post code */
			for (int j21 = 0; j21 < 4; j21 = j21 + 1) {
				fRec17_perm[j21] = fRec17_tmp[vsize + j21];
			}
			/* Vectorizable loop 10 */
			/* Pre code */
			for (int j42 = 0; j42 < 4; j42 = j42 + 1) {
				fYec1_tmp[j42] = fYec1_perm[j42];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec1[i] = fRec27[i] - fRec27[i - 1];
			}
			/* Post code */
			for (int j43 = 0; j43 < 4; j43 = j43 + 1) {
				fYec1_perm[j43] = fYec1_tmp[vsize + j43];
			}
			/* Recursive loop 11 */
			/* Pre code */
			for (int j46 = 0; j46 < 4; j46 = j46 + 1) {
				fRec30_tmp[j46] = fRec30_perm[j46];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec22[i] = ((iZec0[i]) ? 0.0f : fConst18 + fRec30[i - 1]);
				fRec30[i] = fZec22[i] - std::floor(fZec22[i]);
			}
			/* Post code */
			for (int j47 = 0; j47 < 4; j47 = j47 + 1) {
				fRec30_perm[j47] = fRec30_tmp[vsize + j47];
			}
			/* Vectorizable loop 12 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec104[i] = 0.2f * fRec65[i];
			}
			/* Recursive loop 13 */
			/* Pre code */
			for (int j12 = 0; j12 < 4; j12 = j12 + 1) {
				fRec14_tmp[j12] = fRec14_perm[j12];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec2[i] = ((iZec0[i]) ? 0.0f : fRec14[i - 1] + fConst4 * (fZec1[i] + 0.01f));
				fRec14[i] = fZec2[i] - std::floor(fZec2[i]);
			}
			/* Post code */
			for (int j13 = 0; j13 < 4; j13 = j13 + 1) {
				fRec14_perm[j13] = fRec14_tmp[vsize + j13];
			}
			/* Vectorizable loop 14 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec3[i] = 4.6566128e-12f * fRec11[i] * fYec0[i];
			}
			/* Recursive loop 15 */
			/* Pre code */
			for (int j18 = 0; j18 < 4; j18 = j18 + 1) {
				fRec16_tmp[j18] = fRec16_perm[j18];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec5[i] = ((iZec0[i]) ? 0.0f : fRec16[i - 1] + fConst4 * (fZec4[i] + 2.0f));
				fRec16[i] = fZec5[i] - std::floor(fZec5[i]);
			}
			/* Post code */
			for (int j19 = 0; j19 < 4; j19 = j19 + 1) {
				fRec16_perm[j19] = fRec16_tmp[vsize + j19];
			}
			/* Vectorizable loop 16 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = 0.2f * ftbl0MacallaSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec17[i]), 65535))] + 0.8f;
			}
			/* Vectorizable loop 17 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec21[i] = (fYec1[i - 1] <= 0.0f) & (fYec1[i] > 0.0f);
			}
			/* Vectorizable loop 18 */
			/* Pre code */
			for (int j48 = 0; j48 < 4; j48 = j48 + 1) {
				fYec2_tmp[j48] = fYec2_perm[j48];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec2[i] = Macalla_faustpower2_f(2.0f * fRec30[i] + -1.0f);
			}
			/* Post code */
			for (int j49 = 0; j49 < 4; j49 = j49 + 1) {
				fYec2_perm[j49] = fYec2_tmp[vsize + j49];
			}
			/* Recursive loop 19 */
			/* Pre code */
			for (int j58 = 0; j58 < 4; j58 = j58 + 1) {
				fRec33_tmp[j58] = fRec33_perm[j58];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec33[i] = fSlow7 + fConst2 * fRec33[i - 1];
			}
			/* Post code */
			for (int j59 = 0; j59 < 4; j59 = j59 + 1) {
				fRec33_perm[j59] = fRec33_tmp[vsize + j59];
			}
			/* Recursive loop 20 */
			/* Pre code */
			for (int j78 = 0; j78 < 4; j78 = j78 + 1) {
				fRec43_tmp[j78] = fRec43_perm[j78];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec43[i] = fSlow12 + fConst2 * fRec43[i - 1];
			}
			/* Post code */
			for (int j79 = 0; j79 < 4; j79 = j79 + 1) {
				fRec43_perm[j79] = fRec43_tmp[vsize + j79];
			}
			/* Recursive loop 21 */
			/* Pre code */
			for (int j112 = 0; j112 < 4; j112 = j112 + 1) {
				fRec64_tmp[j112] = fRec64_perm[j112];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec105[i] = ((iZec0[i]) ? 0.0f : fRec64[i - 1] + fConst4 * (fZec1[i] + fZec104[i] + 0.01f));
				fRec64[i] = fZec105[i] - std::floor(fZec105[i]);
			}
			/* Post code */
			for (int j113 = 0; j113 < 4; j113 = j113 + 1) {
				fRec64_perm[j113] = fRec64_tmp[vsize + j113];
			}
			/* Recursive loop 22 */
			/* Pre code */
			for (int j116 = 0; j116 < 4; j116 = j116 + 1) {
				fRec67_tmp[j116] = fRec67_perm[j116];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec106[i] = ((iZec0[i]) ? 0.0f : fRec67[i - 1] + fConst4 * (fZec4[i] + fZec104[i] + 2.0f));
				fRec67[i] = fZec106[i] - std::floor(fZec106[i]);
			}
			/* Post code */
			for (int j117 = 0; j117 < 4; j117 = j117 + 1) {
				fRec67_perm[j117] = fRec67_tmp[vsize + j117];
			}
			/* Recursive loop 23 */
			/* Pre code */
			for (int j2 = 0; j2 < 4; j2 = j2 + 1) {
				fRec2_tmp[j2] = fRec2_perm[j2];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec2[i] = std::fmod(fRec2[i - 1] + 4097.0f - fSlow2, 4096.0f);
			}
			/* Post code */
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				fRec2_perm[j3] = fRec2_tmp[vsize + j3];
			}
			/* Recursive loop 24 */
			/* Pre code */
			for (int j16 = 0; j16 < 4; j16 = j16 + 1) {
				fRec12_tmp[j16] = fRec12_perm[j16];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec12[i] = 0.0001f * (fZec3[i] + (1.0f - std::fabs(2.0f * fRec14[i] + -1.0f))) + 0.9999f * fRec12[i - 1];
			}
			/* Post code */
			for (int j17 = 0; j17 < 4; j17 = j17 + 1) {
				fRec12_perm[j17] = fRec12_tmp[vsize + j17];
			}
			/* Recursive loop 25 */
			/* Pre code */
			for (int j22 = 0; j22 < 4; j22 = j22 + 1) {
				fRec15_tmp[j22] = fRec15_perm[j22];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec15[i] = 0.0001f * (fZec3[i] + (1.0f - std::fabs(2.0f * fRec16[i] + -1.0f))) * fZec7[i] + 0.9999f * fRec15[i - 1];
			}
			/* Post code */
			for (int j23 = 0; j23 < 4; j23 = j23 + 1) {
				fRec15_perm[j23] = fRec15_tmp[vsize + j23];
			}
			/* Recursive loop 26 */
			/* Pre code */
			for (int j44 = 0; j44 < 4; j44 = j44 + 1) {
				fRec28_tmp[j44] = fRec28_perm[j44];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec28[i] = fRec28[i - 1] * float(1 - iZec21[i]) + 4.656613e-10f * fYec0[i] * float(iZec21[i]);
			}
			/* Post code */
			for (int j45 = 0; j45 < 4; j45 = j45 + 1) {
				fRec28_perm[j45] = fRec28_tmp[vsize + j45];
			}
			/* Vectorizable loop 27 */
			/* Pre code */
			fYec3_idx = (fYec3_idx + fYec3_idx_save) & 2047;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec3[(i + fYec3_idx) & 2047] = float(iVec0[i - 1]) * (fYec2[i] - fYec2[i - 1]);
			}
			/* Post code */
			fYec3_idx_save = vsize;
			/* Vectorizable loop 28 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec28[i] = std::tan(fConst40 * fRec33[i]);
			}
			/* Recursive loop 29 */
			/* Pre code */
			for (int j62 = 0; j62 < 4; j62 = j62 + 1) {
				fRec34_tmp[j62] = fRec34_perm[j62];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec34[i] = fSlow8 + fConst2 * fRec34[i - 1];
			}
			/* Post code */
			for (int j63 = 0; j63 < 4; j63 = j63 + 1) {
				fRec34_perm[j63] = fRec34_tmp[vsize + j63];
			}
			/* Recursive loop 30 */
			/* Pre code */
			for (int j68 = 0; j68 < 4; j68 = j68 + 1) {
				fRec36_tmp[j68] = fRec36_perm[j68];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec40[i] = ((iZec0[i]) ? 0.0f : fConst42 + fRec36[i - 1]);
				fRec36[i] = fZec40[i] - std::floor(fZec40[i]);
			}
			/* Post code */
			for (int j69 = 0; j69 < 4; j69 = j69 + 1) {
				fRec36_perm[j69] = fRec36_tmp[vsize + j69];
			}
			/* Recursive loop 31 */
			/* Pre code */
			for (int j72 = 0; j72 < 4; j72 = j72 + 1) {
				fRec37_tmp[j72] = fRec37_perm[j72];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec45[i] = ((iZec0[i]) ? 0.0f : fConst47 + fRec37[i - 1]);
				fRec37[i] = fZec45[i] - std::floor(fZec45[i]);
			}
			/* Post code */
			for (int j73 = 0; j73 < 4; j73 = j73 + 1) {
				fRec37_perm[j73] = fRec37_tmp[vsize + j73];
			}
			/* Recursive loop 32 */
			/* Pre code */
			for (int j76 = 0; j76 < 4; j76 = j76 + 1) {
				fRec38_tmp[j76] = fRec38_perm[j76];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec38[i] = std::fmod(fRec38[i - 1] + 4097.0f - fSlow11, 4096.0f);
			}
			/* Post code */
			for (int j77 = 0; j77 < 4; j77 = j77 + 1) {
				fRec38_perm[j77] = fRec38_tmp[vsize + j77];
			}
			/* Vectorizable loop 33 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec50[i] = 4.99f * fRec43[i];
			}
			/* Recursive loop 34 */
			/* Pre code */
			for (int j114 = 0; j114 < 4; j114 = j114 + 1) {
				fRec63_tmp[j114] = fRec63_perm[j114];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec63[i] = 0.0001f * (fZec3[i] + (1.0f - std::fabs(2.0f * fRec64[i] + -1.0f))) + 0.9999f * fRec63[i - 1];
			}
			/* Post code */
			for (int j115 = 0; j115 < 4; j115 = j115 + 1) {
				fRec63_perm[j115] = fRec63_tmp[vsize + j115];
			}
			/* Recursive loop 35 */
			/* Pre code */
			for (int j118 = 0; j118 < 4; j118 = j118 + 1) {
				fRec66_tmp[j118] = fRec66_perm[j118];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec66[i] = 0.0001f * fZec7[i] * (fZec3[i] + (1.0f - std::fabs(2.0f * fRec67[i] + -1.0f))) + 0.9999f * fRec66[i - 1];
			}
			/* Post code */
			for (int j119 = 0; j119 < 4; j119 = j119 + 1) {
				fRec66_perm[j119] = fRec66_tmp[vsize + j119];
			}
			/* Recursive loop 36 */
			/* Pre code */
			for (int j134 = 0; j134 < 4; j134 = j134 + 1) {
				fRec72_tmp[j134] = fRec72_perm[j134];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec118[i] = ((iZec0[i]) ? 0.0f : fRec72[i - 1] + fConst4 * (0.02f * fRec65[i] + 0.15f));
				fRec72[i] = fZec118[i] - std::floor(fZec118[i]);
			}
			/* Post code */
			for (int j135 = 0; j135 < 4; j135 = j135 + 1) {
				fRec72_perm[j135] = fRec72_tmp[vsize + j135];
			}
			/* Recursive loop 37 */
			/* Pre code */
			for (int j138 = 0; j138 < 4; j138 = j138 + 1) {
				fRec73_tmp[j138] = fRec73_perm[j138];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec123[i] = ((iZec0[i]) ? 0.0f : fRec73[i - 1] + fConst4 * (0.03f * fRec65[i] + 0.23f));
				fRec73[i] = fZec123[i] - std::floor(fZec123[i]);
			}
			/* Post code */
			for (int j139 = 0; j139 < 4; j139 = j139 + 1) {
				fRec73_perm[j139] = fRec73_tmp[vsize + j139];
			}
			/* Recursive loop 38 */
			/* Pre code */
			for (int j0 = 0; j0 < 4; j0 = j0 + 1) {
				fRec1_tmp[j0] = fRec1_perm[j0];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec1[i] = fSlow0 + fConst2 * fRec1[i - 1];
			}
			/* Post code */
			for (int j1 = 0; j1 < 4; j1 = j1 + 1) {
				fRec1_perm[j1] = fRec1_tmp[vsize + j1];
			}
			/* Vectorizable loop 39 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec8[i] = fRec11[i] * (6e+02f * fRec12[i] + 1e+02f * fRec15[i]);
			}
			/* Recursive loop 40 */
			/* Pre code */
			for (int j38 = 0; j38 < 4; j38 = j38 + 1) {
				fRec26_tmp[j38] = fRec26_perm[j38];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec26[i] = 0.5221894f * fRec26[i - 3] + 4.656613e-10f * fYec0[i] + 2.494956f * fRec26[i - 1] - 2.0172658f * fRec26[i - 2];
			}
			/* Post code */
			for (int j39 = 0; j39 < 4; j39 = j39 + 1) {
				fRec26_perm[j39] = fRec26_tmp[vsize + j39];
			}
			/* Recursive loop 41 */
			/* Pre code */
			for (int j50 = 0; j50 < 4; j50 = j50 + 1) {
				fRec29_tmp[j50] = fRec29_perm[j50];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec29[i] = 0.999f * fRec29[i - 1] + fConst19 * (fYec3[(i + fYec3_idx) & 2047] - fConst22 * fYec3[(i + fYec3_idx - iConst23) & 2047] - fConst24 * fYec3[(i + fYec3_idx - iConst25) & 2047]);
			}
			/* Post code */
			for (int j51 = 0; j51 < 4; j51 = j51 + 1) {
				fRec29_perm[j51] = fRec29_tmp[vsize + j51];
			}
			/* Recursive loop 42 */
			/* Pre code */
			for (int j52 = 0; j52 < 4; j52 = j52 + 1) {
				fRec31_tmp[j52] = fRec31_perm[j52];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec23[i] = ((iZec0[i]) ? 0.0f : fConst26 + fRec31[i - 1]);
				fRec31[i] = fZec23[i] - std::floor(fZec23[i]);
			}
			/* Post code */
			for (int j53 = 0; j53 < 4; j53 = j53 + 1) {
				fRec31_perm[j53] = fRec31_tmp[vsize + j53];
			}
			/* Recursive loop 43 */
			/* Pre code */
			for (int j54 = 0; j54 < 4; j54 = j54 + 1) {
				fRec32_tmp[j54] = fRec32_perm[j54];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec24[i] = ((iZec0[i]) ? 0.0f : fConst27 + fRec32[i - 1]);
				fRec32[i] = fZec24[i] - std::floor(fZec24[i]);
			}
			/* Post code */
			for (int j55 = 0; j55 < 4; j55 = j55 + 1) {
				fRec32_perm[j55] = fRec32_tmp[vsize + j55];
			}
			/* Vectorizable loop 44 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec25[i] = 0.5f * (fRec28[i] + 1.0f);
			}
			/* Vectorizable loop 45 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec29[i] = 1.0f / fZec28[i];
			}
			/* Vectorizable loop 46 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec35[i] = std::tan(fConst40 * fRec34[i]);
			}
			/* Vectorizable loop 47 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec41[i] = std::max<float>(0.0f, std::min<float>(fConst45, fConst46 * (1.5f * (2.0f * (1.0f - std::fabs(2.0f * fRec36[i] + -1.0f)) + -1.0f) + 7.0f)));
			}
			/* Vectorizable loop 48 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec46[i] = std::max<float>(0.0f, std::min<float>(fConst45, fConst46 * (1.5f * (2.0f * (1.0f - std::fabs(2.0f * fRec37[i] + -1.0f)) + -1.0f) + 11.3f)));
			}
			/* Recursive loop 49 */
			/* Pre code */
			for (int j80 = 0; j80 < 4; j80 = j80 + 1) {
				fRec44_tmp[j80] = fRec44_perm[j80];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec51[i] = ((iZec0[i]) ? 0.0f : fRec44[i - 1] + fConst4 * (fZec50[i] + 0.01f));
				fRec44[i] = fZec51[i] - std::floor(fZec51[i]);
			}
			/* Post code */
			for (int j81 = 0; j81 < 4; j81 = j81 + 1) {
				fRec44_perm[j81] = fRec44_tmp[vsize + j81];
			}
			/* Vectorizable loop 50 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec62[i] = fRec2[i] + 4096.0f;
			}
			/* Vectorizable loop 51 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec77[i] = fRec38[i] + 4096.0f;
			}
			/* Vectorizable loop 52 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec107[i] = fRec11[i] * (6e+02f * fRec63[i] + 1e+02f * fRec66[i]);
			}
			/* Vectorizable loop 53 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec119[i] = std::max<float>(0.0f, std::min<float>(fConst45, fConst46 * (0.5f * fRec65[i] + 1.5f * (2.0f * (1.0f - std::fabs(2.0f * fRec72[i] + -1.0f)) + -1.0f) + 7.0f)));
			}
			/* Vectorizable loop 54 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec124[i] = std::max<float>(0.0f, std::min<float>(fConst45, fConst46 * (0.7f * fRec65[i] + 1.5f * (2.0f * (1.0f - std::fabs(2.0f * fRec73[i] + -1.0f)) + -1.0f) + 11.3f)));
			}
			/* Recursive loop 55 */
			/* Pre code */
			for (int j142 = 0; j142 < 4; j142 = j142 + 1) {
				fRec78_tmp[j142] = fRec78_perm[j142];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec128[i] = ((iZec0[i]) ? 0.0f : fRec78[i - 1] + fConst4 * (fZec50[i] + 2.0f * fRec65[i] + 0.01f));
				fRec78[i] = fZec128[i] - std::floor(fZec128[i]);
			}
			/* Post code */
			for (int j143 = 0; j143 < 4; j143 = j143 + 1) {
				fRec78_perm[j143] = fRec78_tmp[vsize + j143];
			}
			/* Vectorizable loop 56 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = fZec8[i] + 100.00001f;
			}
			/* Recursive loop 57 */
			/* Pre code */
			for (int j26 = 0; j26 < 4; j26 = j26 + 1) {
				iRec20_tmp[j26] = iRec20_perm[j26];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec20[i] = iRec20[i - 1] + 1;
			}
			/* Post code */
			for (int j27 = 0; j27 < 4; j27 = j27 + 1) {
				iRec20_perm[j27] = iRec20_tmp[vsize + j27];
			}
			/* Recursive loop 58 */
			/* Pre code */
			for (int j36 = 0; j36 < 4; j36 = j36 + 1) {
				fRec24_tmp[j36] = fRec24_perm[j36];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec24[i] = fSlow6 + fConst2 * fRec24[i - 1];
			}
			/* Post code */
			for (int j37 = 0; j37 < 4; j37 = j37 + 1) {
				fRec24_perm[j37] = fRec24_tmp[vsize + j37];
			}
			/* Recursive loop 59 */
			/* Pre code */
			for (int j56 = 0; j56 < 4; j56 = j56 + 1) {
				fRec25_tmp[j56] = fRec25_perm[j56];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec25[i] = 1.2f * (0.049922034f * fRec26[i] + 0.0506127f * fRec26[i - 2] - (0.095993534f * fRec26[i - 1] + 0.004408786f * fRec26[i - 3])) + 2.3283064e-10f * fYec0[i - 1] * float((fRec27[i] >= fZec25[i]) * (fRec27[i - 1] < fZec25[i])) + fConst28 * fRec29[i] + 0.01f * (ftbl0MacallaSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec31[i]), 65535))] + ftbl0MacallaSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec32[i]), 65535))]) - fConst36 * (fConst37 * fRec25[i - 1] + fConst38 * fRec25[i - 2]);
			}
			/* Post code */
			for (int j57 = 0; j57 < 4; j57 = j57 + 1) {
				fRec25_perm[j57] = fRec25_tmp[vsize + j57];
			}
			/* Vectorizable loop 60 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec31[i] = Macalla_faustpower2_f(fZec28[i]);
			}
			/* Vectorizable loop 61 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec33[i] = (fZec29[i] + 1.4142135f) / fZec28[i] + 1.0f;
			}
			/* Vectorizable loop 62 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec36[i] = 1.0f / fZec35[i];
			}
			/* Vectorizable loop 63 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec42[i] = fZec41[i] + -1.0f;
			}
			/* Vectorizable loop 64 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec47[i] = fZec46[i] + -1.0f;
			}
			/* Vectorizable loop 65 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec52[i] = std::min<float>(524288.0f, fConst0 * (fSlow14 + fRec43[i] * std::fabs(2.0f * (1.0f - std::fabs(2.0f * fRec44[i] + -1.0f)) + -1.0f)));
			}
			/* Vectorizable loop 66 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec54[i] = std::pow(1e+01f, 0.05f * fRec1[i]);
			}
			/* Vectorizable loop 67 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec55[i] = int(fRec2[i]);
			}
			/* Vectorizable loop 68 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec57[i] = std::floor(fRec2[i]);
			}
			/* Vectorizable loop 69 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec61[i] = std::min<float>(0.001953125f * fRec2[i], 1.0f);
			}
			/* Vectorizable loop 70 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec63[i] = int(fZec62[i]);
			}
			/* Vectorizable loop 71 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec65[i] = std::floor(fZec62[i]);
			}
			/* Vectorizable loop 72 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec70[i] = int(fRec38[i]);
			}
			/* Vectorizable loop 73 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec72[i] = std::floor(fRec38[i]);
			}
			/* Vectorizable loop 74 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec76[i] = std::min<float>(0.001953125f * fRec38[i], 1.0f);
			}
			/* Vectorizable loop 75 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec78[i] = int(fZec77[i]);
			}
			/* Vectorizable loop 76 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec80[i] = std::floor(fZec77[i]);
			}
			/* Recursive loop 77 */
			/* Pre code */
			for (int j92 = 0; j92 < 4; j92 = j92 + 1) {
				fRec49_tmp[j92] = fRec49_perm[j92];
			}
			for (int j94 = 0; j94 < 4; j94 = j94 + 1) {
				fRec50_tmp[j94] = fRec50_perm[j94];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec49[i] = fConst48 * fRec50[i - 1] + fConst49 * fRec49[i - 1];
				fRec50[i] = float(iZec0[i]) + fConst49 * fRec50[i - 1] - fConst48 * fRec49[i - 1];
			}
			/* Post code */
			for (int j93 = 0; j93 < 4; j93 = j93 + 1) {
				fRec49_perm[j93] = fRec49_tmp[vsize + j93];
			}
			for (int j95 = 0; j95 < 4; j95 = j95 + 1) {
				fRec50_perm[j95] = fRec50_tmp[vsize + j95];
			}
			/* Vectorizable loop 78 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec108[i] = fZec107[i] + 100.00001f;
			}
			/* Vectorizable loop 79 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec120[i] = fZec119[i] + -1.0f;
			}
			/* Vectorizable loop 80 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec125[i] = fZec124[i] + -1.0f;
			}
			/* Vectorizable loop 81 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec129[i] = std::min<float>(524288.0f, fSlow28 + 4e+01f * fRec65[i] + fRec43[i] * (fConst0 + 1e+03f * fRec65[i]) * std::fabs(2.0f * (1.0f - std::fabs(2.0f * fRec78[i] + -1.0f)) + -1.0f));
			}
			/* Recursive loop 82 */
			/* Pre code */
			for (int j4 = 0; j4 < 4; j4 = j4 + 1) {
				fRec3_tmp[j4] = fRec3_perm[j4];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec3[i] = fSlow3 + fConst2 * fRec3[i - 1];
			}
			/* Post code */
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				fRec3_perm[j5] = fRec3_tmp[vsize + j5];
			}
			/* Vectorizable loop 83 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec10[i] = std::floor(fZec9[i]);
			}
			/* Vectorizable loop 84 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec11[i] = int(fZec9[i]);
			}
			/* Vectorizable loop 85 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec12[i] = (iRec20[i - 1] % iConst11) == 0;
			}
			/* Vectorizable loop 86 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec26[i] = Macalla_faustpower2_f(fRec24[i]);
			}
			/* Vectorizable loop 87 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec27[i] = fRec25[i] - fRec25[i - 2];
			}
			/* Vectorizable loop 88 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec30[i] = (fZec29[i] + -1.4142135f) / fZec28[i] + 1.0f;
			}
			/* Vectorizable loop 89 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec32[i] = 1.0f - 1.0f / fZec31[i];
			}
			/* Vectorizable loop 90 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec37[i] = (fZec36[i] + -1.4142135f) / fZec35[i] + 1.0f;
			}
			/* Vectorizable loop 91 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec34[i] = fZec31[i] * fZec33[i];
			}
			/* Vectorizable loop 92 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec38[i] = 1.0f - 1.0f / Macalla_faustpower2_f(fZec35[i]);
			}
			/* Vectorizable loop 93 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec39[i] = (fZec36[i] + 1.4142135f) / fZec35[i] + 1.0f;
			}
			/* Recursive loop 94 */
			/* Pre code */
			for (int j66 = 0; j66 < 4; j66 = j66 + 1) {
				fRec35_tmp[j66] = fRec35_perm[j66];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec35[i] = fSlow9 + fConst2 * fRec35[i - 1];
			}
			/* Post code */
			for (int j67 = 0; j67 < 4; j67 = j67 + 1) {
				fRec35_perm[j67] = fRec35_tmp[vsize + j67];
			}
			/* Vectorizable loop 95 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec44[i] = std::floor(fZec42[i]);
			}
			/* Vectorizable loop 96 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec43[i] = int(fZec42[i]);
			}
			/* Vectorizable loop 97 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec48[i] = int(fZec47[i]);
			}
			/* Vectorizable loop 98 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec49[i] = std::floor(fZec47[i]);
			}
			/* Recursive loop 99 */
			/* Pre code */
			for (int j82 = 0; j82 < 4; j82 = j82 + 1) {
				fRec39_tmp[j82] = fRec39_perm[j82];
			}
			for (int j84 = 0; j84 < 4; j84 = j84 + 1) {
				fRec40_tmp[j84] = fRec40_perm[j84];
			}
			for (int j86 = 0; j86 < 4; j86 = j86 + 1) {
				fRec41_tmp[j86] = fRec41_perm[j86];
			}
			for (int j88 = 0; j88 < 4; j88 = j88 + 1) {
				fRec42_tmp[j88] = fRec42_perm[j88];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec53[i] = ((fRec39[i - 1] != 0.0f) ? (((fRec40[i - 1] > 0.0f) & (fRec40[i - 1] < 1.0f)) ? fRec39[i - 1] : 0.0f) : (((fRec40[i - 1] == 0.0f) & (fZec52[i] != fRec41[i - 1])) ? fConst16 : (((fRec40[i - 1] == 1.0f) & (fZec52[i] != fRec42[i - 1])) ? -fConst16 : 0.0f)));
				fRec39[i] = fZec53[i];
				fRec40[i] = std::max<float>(0.0f, std::min<float>(1.0f, fRec40[i - 1] + fZec53[i]));
				fRec41[i] = (((fRec40[i - 1] >= 1.0f) & (fRec42[i - 1] != fZec52[i])) ? fZec52[i] : fRec41[i - 1]);
				fRec42[i] = (((fRec40[i - 1] <= 0.0f) & (fRec41[i - 1] != fZec52[i])) ? fZec52[i] : fRec42[i - 1]);
			}
			/* Post code */
			for (int j83 = 0; j83 < 4; j83 = j83 + 1) {
				fRec39_perm[j83] = fRec39_tmp[vsize + j83];
			}
			for (int j85 = 0; j85 < 4; j85 = j85 + 1) {
				fRec40_perm[j85] = fRec40_tmp[vsize + j85];
			}
			for (int j87 = 0; j87 < 4; j87 = j87 + 1) {
				fRec41_perm[j87] = fRec41_tmp[vsize + j87];
			}
			for (int j89 = 0; j89 < 4; j89 = j89 + 1) {
				fRec42_perm[j89] = fRec42_tmp[vsize + j89];
			}
			/* Vectorizable loop 100 */
			/* Pre code */
			fYec6_idx = (fYec6_idx + fYec6_idx_save) & 131071;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec6[(i + fYec6_idx) & 131071] = float(input0[i]) * fZec54[i];
			}
			/* Post code */
			fYec6_idx_save = vsize;
			/* Vectorizable loop 101 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec56[i] = std::min<int>(65537, std::max<int>(0, iZec55[i]));
			}
			/* Vectorizable loop 102 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec58[i] = fZec57[i] + (1.0f - fRec2[i]);
			}
			/* Vectorizable loop 103 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec59[i] = fRec2[i] - fZec57[i];
			}
			/* Vectorizable loop 104 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec60[i] = std::min<int>(65537, std::max<int>(0, iZec55[i] + 1));
			}
			/* Vectorizable loop 105 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec64[i] = std::min<int>(65537, std::max<int>(0, iZec63[i]));
			}
			/* Vectorizable loop 106 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec66[i] = fZec65[i] + (-4095.0f - fRec2[i]);
			}
			/* Vectorizable loop 107 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec67[i] = fRec2[i] + (4096.0f - fZec65[i]);
			}
			/* Vectorizable loop 108 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec68[i] = std::min<int>(65537, std::max<int>(0, iZec63[i] + 1));
			}
			/* Vectorizable loop 109 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec69[i] = 1.0f - fZec61[i];
			}
			/* Vectorizable loop 110 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec71[i] = std::min<int>(65537, std::max<int>(0, iZec70[i]));
			}
			/* Vectorizable loop 111 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec73[i] = fZec72[i] + (1.0f - fRec38[i]);
			}
			/* Vectorizable loop 112 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec74[i] = fRec38[i] - fZec72[i];
			}
			/* Vectorizable loop 113 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec75[i] = std::min<int>(65537, std::max<int>(0, iZec70[i] + 1));
			}
			/* Vectorizable loop 114 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec79[i] = std::min<int>(65537, std::max<int>(0, iZec78[i]));
			}
			/* Vectorizable loop 115 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec81[i] = fZec80[i] + (-4095.0f - fRec38[i]);
			}
			/* Vectorizable loop 116 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec82[i] = fRec38[i] + (4096.0f - fZec80[i]);
			}
			/* Vectorizable loop 117 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec83[i] = std::min<int>(65537, std::max<int>(0, iZec78[i] + 1));
			}
			/* Vectorizable loop 118 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec84[i] = 1.0f - fZec76[i];
			}
			/* Vectorizable loop 119 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec86[i] = 2984.513f * (1.0f - fRec49[i]) + 314.15927f;
			}
			/* Vectorizable loop 120 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec109[i] = std::floor(fZec108[i]);
			}
			/* Vectorizable loop 121 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec110[i] = int(fZec108[i]);
			}
			/* Vectorizable loop 122 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec121[i] = int(fZec120[i]);
			}
			/* Vectorizable loop 123 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec122[i] = std::floor(fZec120[i]);
			}
			/* Vectorizable loop 124 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec126[i] = int(fZec125[i]);
			}
			/* Vectorizable loop 125 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec127[i] = std::floor(fZec125[i]);
			}
			/* Recursive loop 126 */
			/* Pre code */
			for (int j144 = 0; j144 < 4; j144 = j144 + 1) {
				fRec74_tmp[j144] = fRec74_perm[j144];
			}
			for (int j146 = 0; j146 < 4; j146 = j146 + 1) {
				fRec75_tmp[j146] = fRec75_perm[j146];
			}
			for (int j148 = 0; j148 < 4; j148 = j148 + 1) {
				fRec76_tmp[j148] = fRec76_perm[j148];
			}
			for (int j150 = 0; j150 < 4; j150 = j150 + 1) {
				fRec77_tmp[j150] = fRec77_perm[j150];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec130[i] = ((fRec74[i - 1] != 0.0f) ? (((fRec75[i - 1] > 0.0f) & (fRec75[i - 1] < 1.0f)) ? fRec74[i - 1] : 0.0f) : (((fRec75[i - 1] == 0.0f) & (fZec129[i] != fRec76[i - 1])) ? fConst16 : (((fRec75[i - 1] == 1.0f) & (fZec129[i] != fRec77[i - 1])) ? -fConst16 : 0.0f)));
				fRec74[i] = fZec130[i];
				fRec75[i] = std::max<float>(0.0f, std::min<float>(1.0f, fRec75[i - 1] + fZec130[i]));
				fRec76[i] = (((fRec75[i - 1] >= 1.0f) & (fRec77[i - 1] != fZec129[i])) ? fZec129[i] : fRec76[i - 1]);
				fRec77[i] = (((fRec75[i - 1] <= 0.0f) & (fRec76[i - 1] != fZec129[i])) ? fZec129[i] : fRec77[i - 1]);
			}
			/* Post code */
			for (int j145 = 0; j145 < 4; j145 = j145 + 1) {
				fRec74_perm[j145] = fRec74_tmp[vsize + j145];
			}
			for (int j147 = 0; j147 < 4; j147 = j147 + 1) {
				fRec75_perm[j147] = fRec75_tmp[vsize + j147];
			}
			for (int j149 = 0; j149 < 4; j149 = j149 + 1) {
				fRec76_perm[j149] = fRec76_tmp[vsize + j149];
			}
			for (int j151 = 0; j151 < 4; j151 = j151 + 1) {
				fRec77_perm[j151] = fRec77_tmp[vsize + j151];
			}
			/* Vectorizable loop 127 */
			/* Pre code */
			fYec10_idx = (fYec10_idx + fYec10_idx_save) & 131071;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec10[(i + fYec10_idx) & 131071] = float(input1[i]) * fZec54[i];
			}
			/* Post code */
			fYec10_idx_save = vsize;
			/* Recursive loop 128 */
			/* Pre code */
			for (int j24 = 0; j24 < 4; j24 = j24 + 1) {
				fRec10_tmp[j24] = fRec10_perm[j24];
			}
			for (int j28 = 0; j28 < 4; j28 = j28 + 1) {
				fRec19_tmp[j28] = fRec19_perm[j28];
			}
			for (int j30 = 0; j30 < 4; j30 = j30 + 1) {
				fRec22_tmp[j30] = fRec22_perm[j30];
			}
			for (int j32 = 0; j32 < 4; j32 = j32 + 1) {
				fRec21_tmp[j32] = fRec21_perm[j32];
			}
			for (int j34 = 0; j34 < 4; j34 = j34 + 1) {
				fRec23_tmp[j34] = fRec23_perm[j34];
			}
			for (int j60 = 0; j60 < 4; j60 = j60 + 1) {
				fRec9_tmp[j60] = fRec9_perm[j60];
			}
			for (int j64 = 0; j64 < 4; j64 = j64 + 1) {
				fRec8_tmp[j64] = fRec8_perm[j64];
			}
			fYec4_idx = (fYec4_idx + fYec4_idx_save) & 2047;
			for (int j70 = 0; j70 < 4; j70 = j70 + 1) {
				fRec6_tmp[j70] = fRec6_perm[j70];
			}
			fYec5_idx = (fYec5_idx + fYec5_idx_save) & 4095;
			for (int j74 = 0; j74 < 4; j74 = j74 + 1) {
				fRec4_tmp[j74] = fRec4_perm[j74];
			}
			fYec7_idx = (fYec7_idx + fYec7_idx_save) & 1048575;
			fRec0_idx = (fRec0_idx + fRec0_idx_save) & 262143;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec10[i] = (fZec8[i] + (1e+02f - fZec10[i])) * fRec0[(i + fRec0_idx - (int(std::min<float>(fConst0, float(std::max<int>(0, iZec11[i] + 1)))) + 1)) & 262143] - (fRec0[(i + fRec0_idx - (int(std::min<float>(fConst0, float(std::max<int>(0, iZec11[i])))) + 1)) & 262143] * (fZec8[i] + (99.0f - fZec10[i])) + fConst8 * (fConst9 * fRec10[i - 2] + fConst10 * fRec10[i - 1]));
				fZec13[i] = fConst8 * (fRec10[i - 2] + fRec10[i] + 2.0f * fRec10[i - 1]);
				fRec19[i] = ((iZec12[i]) ? 0.003921569f * std::floor(255.0f * std::fabs(fZec13[i]) + 0.5f) * (float(2 * (fZec13[i] > 0.0f)) + -1.0f) : fRec19[i - 1]);
				fZec14[i] = ((iSlow5) ? fRec19[i] : fZec13[i]);
				fZec15[i] = std::fabs(fZec14[i]);
				fZec16[i] = ((fZec15[i] > fRec22[i - 1]) ? fConst13 : fConst12);
				fRec22[i] = fZec15[i] * (1.0f - fZec16[i]) + fRec22[i - 1] * fZec16[i];
				fRec21[i] = fConst14 * fRec21[i - 1] - fConst15 * std::max<float>(2e+01f * std::log10(std::max<float>(1.1754944e-38f, fRec22[i])) + 6.0f, 0.0f);
				fZec17[i] = fZec14[i] * std::pow(1e+01f, 0.05f * fRec21[i]);
				fZec18[i] = std::fabs(fZec17[i]);
				fZec19[i] = ((fZec18[i] > fRec23[i - 1]) ? fConst17 : fConst12);
				fRec23[i] = fZec18[i] * (1.0f - fZec19[i]) + fRec23[i - 1] * fZec19[i];
				fRec9[i] = fZec17[i] + fConst39 * fRec23[i] * fZec26[i] * fZec27[i] - (fRec9[i - 2] * fZec30[i] + 2.0f * fRec9[i - 1] * fZec32[i]) / fZec33[i];
				fRec8[i] = (fRec9[i - 2] + (fRec9[i] - 2.0f * fRec9[i - 1])) / fZec34[i] - (fRec8[i - 2] * fZec37[i] + 2.0f * fRec8[i - 1] * fZec38[i]) / fZec39[i];
				fYec4[(i + fYec4_idx) & 2047] = 0.98f * ((fRec8[i - 2] + fRec8[i] + 2.0f * fRec8[i - 1]) / fZec39[i]) - fRec35[i] * fRec6[i - 1];
				fRec6[i] = fYec4[(i + fYec4_idx - std::min<int>(iConst44, std::max<int>(0, iZec43[i]))) & 2047] * (fZec44[i] + (2.0f - fZec41[i])) + (fZec41[i] + (-1.0f - fZec44[i])) * fYec4[(i + fYec4_idx - std::min<int>(iConst44, std::max<int>(0, iZec43[i] + 1))) & 2047];
				fRec7[i] = fRec35[i] * fYec4[(i + fYec4_idx) & 2047];
				fYec5[(i + fYec5_idx) & 4095] = fRec7[i] + fRec6[i - 1] - 0.9f * fRec35[i] * fRec4[i - 1];
				fRec4[i] = fYec5[(i + fYec5_idx - std::min<int>(iConst44, std::max<int>(0, iZec48[i]))) & 4095] * (fZec49[i] + (2.0f - fZec46[i])) + (fZec46[i] + (-1.0f - fZec49[i])) * fYec5[(i + fYec5_idx - std::min<int>(iConst44, std::max<int>(0, iZec48[i] + 1))) & 4095];
				fRec5[i] = 0.9f * fRec35[i] * fYec5[(i + fYec5_idx) & 4095];
				fYec7[(i + fYec7_idx) & 1048575] = ((iSlow15) ? (fYec6[(i + fYec6_idx - iZec56[i]) & 131071] * fZec58[i] + fZec59[i] * fYec6[(i + fYec6_idx - iZec60[i]) & 131071]) * fZec61[i] + (fYec6[(i + fYec6_idx - iZec64[i]) & 131071] * fZec66[i] + fZec67[i] * fYec6[(i + fYec6_idx - iZec68[i]) & 131071]) * fZec69[i] : fYec6[(i + fYec6_idx) & 131071]) + fRec3[i] * (fRec5[i] + fRec4[i - 1]) + ((iSlow16) ? (fYec6[(i + fYec6_idx - iZec71[i]) & 131071] * fZec73[i] + fZec74[i] * fYec6[(i + fYec6_idx - iZec75[i]) & 131071]) * fZec76[i] + (fYec6[(i + fYec6_idx - iZec79[i]) & 131071] * fZec81[i] + fZec82[i] * fYec6[(i + fYec6_idx - iZec83[i]) & 131071]) * fZec84[i] : fYec6[(i + fYec6_idx) & 131071]);
				fZec85[i] = fYec7[(i + fYec7_idx - int(std::min<float>(524288.0f, std::max<float>(0.0f, fRec41[i])))) & 1048575];
				fRec0[(i + fRec0_idx) & 262143] = fZec85[i] + fRec40[i] * (fYec7[(i + fYec7_idx - int(std::min<float>(524288.0f, std::max<float>(0.0f, fRec42[i])))) & 1048575] - fZec85[i]);
			}
			/* Post code */
			fYec7_idx_save = vsize;
			fYec5_idx_save = vsize;
			fYec4_idx_save = vsize;
			for (int j35 = 0; j35 < 4; j35 = j35 + 1) {
				fRec23_perm[j35] = fRec23_tmp[vsize + j35];
			}
			for (int j31 = 0; j31 < 4; j31 = j31 + 1) {
				fRec22_perm[j31] = fRec22_tmp[vsize + j31];
			}
			for (int j33 = 0; j33 < 4; j33 = j33 + 1) {
				fRec21_perm[j33] = fRec21_tmp[vsize + j33];
			}
			for (int j29 = 0; j29 < 4; j29 = j29 + 1) {
				fRec19_perm[j29] = fRec19_tmp[vsize + j29];
			}
			for (int j25 = 0; j25 < 4; j25 = j25 + 1) {
				fRec10_perm[j25] = fRec10_tmp[vsize + j25];
			}
			for (int j61 = 0; j61 < 4; j61 = j61 + 1) {
				fRec9_perm[j61] = fRec9_tmp[vsize + j61];
			}
			for (int j65 = 0; j65 < 4; j65 = j65 + 1) {
				fRec8_perm[j65] = fRec8_tmp[vsize + j65];
			}
			for (int j71 = 0; j71 < 4; j71 = j71 + 1) {
				fRec6_perm[j71] = fRec6_tmp[vsize + j71];
			}
			for (int j75 = 0; j75 < 4; j75 = j75 + 1) {
				fRec4_perm[j75] = fRec4_tmp[vsize + j75];
			}
			fRec0_idx_save = vsize;
			/* Recursive loop 129 */
			/* Pre code */
			for (int j90 = 0; j90 < 4; j90 = j90 + 1) {
				fRec45_tmp[j90] = fRec45_perm[j90];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec45[i] = fSlow17 + fConst2 * fRec45[i - 1];
			}
			/* Post code */
			for (int j91 = 0; j91 < 4; j91 = j91 + 1) {
				fRec45_perm[j91] = fRec45_tmp[vsize + j91];
			}
			/* Vectorizable loop 130 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec87[i] = std::cos(fConst52 * fZec86[i]);
			}
			/* Vectorizable loop 131 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec89[i] = std::cos(fConst54 * fZec86[i]);
			}
			/* Recursive loop 132 */
			/* Pre code */
			for (int j120 = 0; j120 < 4; j120 = j120 + 1) {
				fRec62_tmp[j120] = fRec62_perm[j120];
			}
			for (int j122 = 0; j122 < 4; j122 = j122 + 1) {
				fRec68_tmp[j122] = fRec68_perm[j122];
			}
			for (int j124 = 0; j124 < 4; j124 = j124 + 1) {
				fRec70_tmp[j124] = fRec70_perm[j124];
			}
			for (int j126 = 0; j126 < 4; j126 = j126 + 1) {
				fRec69_tmp[j126] = fRec69_perm[j126];
			}
			for (int j128 = 0; j128 < 4; j128 = j128 + 1) {
				fRec71_tmp[j128] = fRec71_perm[j128];
			}
			for (int j130 = 0; j130 < 4; j130 = j130 + 1) {
				fRec61_tmp[j130] = fRec61_perm[j130];
			}
			for (int j132 = 0; j132 < 4; j132 = j132 + 1) {
				fRec60_tmp[j132] = fRec60_perm[j132];
			}
			fYec8_idx = (fYec8_idx + fYec8_idx_save) & 16383;
			for (int j136 = 0; j136 < 4; j136 = j136 + 1) {
				fRec58_tmp[j136] = fRec58_perm[j136];
			}
			fYec9_idx = (fYec9_idx + fYec9_idx_save) & 16383;
			for (int j140 = 0; j140 < 4; j140 = j140 + 1) {
				fRec56_tmp[j140] = fRec56_perm[j140];
			}
			fYec11_idx = (fYec11_idx + fYec11_idx_save) & 1048575;
			fRec55_idx = (fRec55_idx + fRec55_idx_save) & 262143;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec62[i] = (fZec107[i] + (1e+02f - fZec109[i])) * fRec55[(i + fRec55_idx - (int(std::min<float>(fConst0, float(std::max<int>(0, iZec110[i] + 1)))) + 1)) & 262143] - (fRec55[(i + fRec55_idx - (int(std::min<float>(fConst0, float(std::max<int>(0, iZec110[i])))) + 1)) & 262143] * (fZec107[i] + (99.0f - fZec109[i])) + fConst8 * (fConst9 * fRec62[i - 2] + fConst10 * fRec62[i - 1]));
				fZec111[i] = fConst8 * (fRec62[i - 2] + fRec62[i] + 2.0f * fRec62[i - 1]);
				fRec68[i] = ((iZec12[i]) ? 0.003921569f * std::floor(255.0f * std::fabs(fZec111[i]) + 0.5f) * (float(2 * (fZec111[i] > 0.0f)) + -1.0f) : fRec68[i - 1]);
				fZec112[i] = ((iSlow5) ? fRec68[i] : fZec111[i]);
				fZec113[i] = std::fabs(fZec112[i]);
				fZec114[i] = ((fZec113[i] > fRec70[i - 1]) ? fConst13 : fConst12);
				fRec70[i] = fZec113[i] * (1.0f - fZec114[i]) + fRec70[i - 1] * fZec114[i];
				fRec69[i] = fConst14 * fRec69[i - 1] - fConst15 * std::max<float>(2e+01f * std::log10(std::max<float>(1.1754944e-38f, fRec70[i])) + 6.0f, 0.0f);
				fZec115[i] = fZec112[i] * std::pow(1e+01f, 0.05f * fRec69[i]);
				fZec116[i] = std::fabs(fZec115[i]);
				fZec117[i] = ((fZec116[i] > fRec71[i - 1]) ? fConst17 : fConst12);
				fRec71[i] = fZec116[i] * (1.0f - fZec117[i]) + fRec71[i - 1] * fZec117[i];
				fRec61[i] = fZec115[i] + fConst39 * fZec26[i] * fRec71[i] * fZec27[i] - (fZec30[i] * fRec61[i - 2] + 2.0f * fZec32[i] * fRec61[i - 1]) / fZec33[i];
				fRec60[i] = (fRec61[i - 2] + (fRec61[i] - 2.0f * fRec61[i - 1])) / fZec34[i] - (fZec37[i] * fRec60[i - 2] + 2.0f * fZec38[i] * fRec60[i - 1]) / fZec39[i];
				fYec8[(i + fYec8_idx) & 16383] = 0.98f * ((fRec60[i - 2] + fRec60[i] + 2.0f * fRec60[i - 1]) / fZec39[i]) - fRec35[i] * fRec58[i - 1];
				fRec58[i] = fYec8[(i + fYec8_idx - std::min<int>(iConst44, std::max<int>(0, iZec121[i]))) & 16383] * (fZec122[i] + (2.0f - fZec119[i])) + (fZec119[i] + (-1.0f - fZec122[i])) * fYec8[(i + fYec8_idx - std::min<int>(iConst44, std::max<int>(0, iZec121[i] + 1))) & 16383];
				fRec59[i] = fRec35[i] * fYec8[(i + fYec8_idx) & 16383];
				fYec9[(i + fYec9_idx) & 16383] = fRec59[i] + fRec58[i - 1] - 0.9f * fRec35[i] * fRec56[i - 1];
				fRec56[i] = fYec9[(i + fYec9_idx - std::min<int>(iConst44, std::max<int>(0, iZec126[i]))) & 16383] * (fZec127[i] + (2.0f - fZec124[i])) + (fZec124[i] + (-1.0f - fZec127[i])) * fYec9[(i + fYec9_idx - std::min<int>(iConst44, std::max<int>(0, iZec126[i] + 1))) & 16383];
				fRec57[i] = 0.9f * fRec35[i] * fYec9[(i + fYec9_idx) & 16383];
				fYec11[(i + fYec11_idx) & 1048575] = ((iSlow15) ? fZec61[i] * (fZec58[i] * fYec10[(i + fYec10_idx - iZec56[i]) & 131071] + fZec59[i] * fYec10[(i + fYec10_idx - iZec60[i]) & 131071]) + fZec69[i] * (fZec66[i] * fYec10[(i + fYec10_idx - iZec64[i]) & 131071] + fZec67[i] * fYec10[(i + fYec10_idx - iZec68[i]) & 131071]) : fYec10[(i + fYec10_idx) & 131071]) + fRec3[i] * (fRec57[i] + fRec56[i - 1]) + ((iSlow16) ? fZec76[i] * (fZec73[i] * fYec10[(i + fYec10_idx - iZec71[i]) & 131071] + fZec74[i] * fYec10[(i + fYec10_idx - iZec75[i]) & 131071]) + fZec84[i] * (fZec81[i] * fYec10[(i + fYec10_idx - iZec79[i]) & 131071] + fZec82[i] * fYec10[(i + fYec10_idx - iZec83[i]) & 131071]) : fYec10[(i + fYec10_idx) & 131071]);
				fZec131[i] = fYec11[(i + fYec11_idx - int(std::min<float>(524288.0f, std::max<float>(0.0f, fRec76[i])))) & 1048575];
				fRec55[(i + fRec55_idx) & 262143] = fZec131[i] + fRec75[i] * (fYec11[(i + fYec11_idx - int(std::min<float>(524288.0f, std::max<float>(0.0f, fRec77[i])))) & 1048575] - fZec131[i]);
			}
			/* Post code */
			fYec11_idx_save = vsize;
			fYec9_idx_save = vsize;
			fYec8_idx_save = vsize;
			for (int j129 = 0; j129 < 4; j129 = j129 + 1) {
				fRec71_perm[j129] = fRec71_tmp[vsize + j129];
			}
			for (int j125 = 0; j125 < 4; j125 = j125 + 1) {
				fRec70_perm[j125] = fRec70_tmp[vsize + j125];
			}
			for (int j127 = 0; j127 < 4; j127 = j127 + 1) {
				fRec69_perm[j127] = fRec69_tmp[vsize + j127];
			}
			for (int j123 = 0; j123 < 4; j123 = j123 + 1) {
				fRec68_perm[j123] = fRec68_tmp[vsize + j123];
			}
			for (int j121 = 0; j121 < 4; j121 = j121 + 1) {
				fRec62_perm[j121] = fRec62_tmp[vsize + j121];
			}
			for (int j131 = 0; j131 < 4; j131 = j131 + 1) {
				fRec61_perm[j131] = fRec61_tmp[vsize + j131];
			}
			for (int j133 = 0; j133 < 4; j133 = j133 + 1) {
				fRec60_perm[j133] = fRec60_tmp[vsize + j133];
			}
			for (int j137 = 0; j137 < 4; j137 = j137 + 1) {
				fRec58_perm[j137] = fRec58_tmp[vsize + j137];
			}
			for (int j141 = 0; j141 < 4; j141 = j141 + 1) {
				fRec56_perm[j141] = fRec56_tmp[vsize + j141];
			}
			fRec55_idx_save = vsize;
			/* Recursive loop 133 */
			/* Pre code */
			for (int j96 = 0; j96 < 4; j96 = j96 + 1) {
				fRec48_tmp[j96] = fRec48_perm[j96];
			}
			for (int j98 = 0; j98 < 4; j98 = j98 + 1) {
				fRec47_tmp[j98] = fRec47_perm[j98];
			}
			for (int j100 = 0; j100 < 4; j100 = j100 + 1) {
				fRec46_tmp[j100] = fRec46_perm[j100];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec88[i] = fRec48[i - 1] * fZec87[i];
				fRec48[i] = fRec0[(i + fRec0_idx) & 262143] + 0.23f * fRec46[i - 1] + fConst51 * fZec88[i] - fConst53 * fRec48[i - 2];
				fZec90[i] = fRec47[i - 1] * fZec89[i];
				fRec47[i] = fRec48[i - 2] + fConst53 * (fRec48[i] - fRec47[i - 2]) - fConst51 * (fZec88[i] - fZec90[i]);
				fRec46[i] = fRec47[i - 2] + fConst53 * fRec47[i] - fConst51 * fZec90[i];
			}
			/* Post code */
			for (int j97 = 0; j97 < 4; j97 = j97 + 1) {
				fRec48_perm[j97] = fRec48_tmp[vsize + j97];
			}
			for (int j99 = 0; j99 < 4; j99 = j99 + 1) {
				fRec47_perm[j99] = fRec47_tmp[vsize + j99];
			}
			for (int j101 = 0; j101 < 4; j101 = j101 + 1) {
				fRec46_perm[j101] = fRec46_tmp[vsize + j101];
			}
			/* Vectorizable loop 134 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec91[i] = 1.0f - 0.5f * fRec45[i];
			}
			/* Recursive loop 135 */
			/* Pre code */
			for (int j152 = 0; j152 < 4; j152 = j152 + 1) {
				fRec81_tmp[j152] = fRec81_perm[j152];
			}
			for (int j154 = 0; j154 < 4; j154 = j154 + 1) {
				fRec80_tmp[j154] = fRec80_perm[j154];
			}
			for (int j156 = 0; j156 < 4; j156 = j156 + 1) {
				fRec79_tmp[j156] = fRec79_perm[j156];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec132[i] = fZec87[i] * fRec81[i - 1];
				fRec81[i] = fRec55[(i + fRec55_idx) & 262143] + 0.23f * fRec79[i - 1] + fConst51 * fZec132[i] - fConst53 * fRec81[i - 2];
				fZec133[i] = fZec89[i] * fRec80[i - 1];
				fRec80[i] = fRec81[i - 2] + fConst53 * (fRec81[i] - fRec80[i - 2]) - fConst51 * (fZec132[i] - fZec133[i]);
				fRec79[i] = fRec80[i - 2] + fConst53 * fRec80[i] - fConst51 * fZec133[i];
			}
			/* Post code */
			for (int j153 = 0; j153 < 4; j153 = j153 + 1) {
				fRec81_perm[j153] = fRec81_tmp[vsize + j153];
			}
			for (int j155 = 0; j155 < 4; j155 = j155 + 1) {
				fRec80_perm[j155] = fRec80_tmp[vsize + j155];
			}
			for (int j157 = 0; j157 < 4; j157 = j157 + 1) {
				fRec79_perm[j157] = fRec79_tmp[vsize + j157];
			}
			/* Vectorizable loop 136 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec92[i] = fRec0[(i + fRec0_idx) & 262143] * fZec91[i] + 0.5f * fRec45[i] * fRec46[i];
			}
			/* Vectorizable loop 137 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec95[i] = std::fabs(float(input0[i]));
			}
			/* Recursive loop 138 */
			/* Pre code */
			for (int j108 = 0; j108 < 4; j108 = j108 + 1) {
				fRec54_tmp[j108] = fRec54_perm[j108];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec54[i] = fSlow24 + fConst2 * fRec54[i - 1];
			}
			/* Post code */
			for (int j109 = 0; j109 < 4; j109 = j109 + 1) {
				fRec54_perm[j109] = fRec54_tmp[vsize + j109];
			}
			/* Vectorizable loop 139 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec134[i] = fRec55[(i + fRec55_idx) & 262143] * fZec91[i] + 0.5f * fRec45[i] * fRec79[i];
			}
			/* Vectorizable loop 140 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec137[i] = std::fabs(float(input1[i]));
			}
			/* Vectorizable loop 141 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec93[i] = std::fabs(fZec92[i]);
			}
			/* Recursive loop 142 */
			/* Pre code */
			for (int j106 = 0; j106 < 4; j106 = j106 + 1) {
				fRec53_tmp[j106] = fRec53_perm[j106];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec96[i] = ((fZec95[i] > fRec53[i - 1]) ? fSlow23 : fSlow20);
				fRec53[i] = fZec95[i] * (1.0f - fZec96[i]) + fRec53[i - 1] * fZec96[i];
			}
			/* Post code */
			for (int j107 = 0; j107 < 4; j107 = j107 + 1) {
				fRec53_perm[j107] = fRec53_tmp[vsize + j107];
			}
			/* Vectorizable loop 143 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec98[i] = 5e+01f * fRec54[i];
			}
			/* Vectorizable loop 144 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec135[i] = std::fabs(fZec134[i]);
			}
			/* Recursive loop 145 */
			/* Pre code */
			for (int j162 = 0; j162 < 4; j162 = j162 + 1) {
				fRec84_tmp[j162] = fRec84_perm[j162];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec138[i] = ((fZec137[i] > fRec84[i - 1]) ? fSlow23 : fSlow20);
				fRec84[i] = fZec137[i] * (1.0f - fZec138[i]) + fRec84[i - 1] * fZec138[i];
			}
			/* Post code */
			for (int j163 = 0; j163 < 4; j163 = j163 + 1) {
				fRec84_perm[j163] = fRec84_tmp[vsize + j163];
			}
			/* Recursive loop 146 */
			/* Pre code */
			for (int j102 = 0; j102 < 4; j102 = j102 + 1) {
				fRec52_tmp[j102] = fRec52_perm[j102];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec94[i] = ((fZec93[i] > fRec52[i - 1]) ? fConst13 : fConst12);
				fRec52[i] = fZec93[i] * (1.0f - fZec94[i]) + fRec52[i - 1] * fZec94[i];
			}
			/* Post code */
			for (int j103 = 0; j103 < 4; j103 = j103 + 1) {
				fRec52_perm[j103] = fRec52_tmp[vsize + j103];
			}
			/* Vectorizable loop 147 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec97[i] = 2e+01f * std::log10(std::max<float>(1.1754944e-38f, fRec53[i]));
			}
			/* Vectorizable loop 148 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec99[i] = -1.5f - fZec98[i];
			}
			/* Vectorizable loop 149 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec100[i] = 1.5f - fZec98[i];
			}
			/* Recursive loop 150 */
			/* Pre code */
			for (int j158 = 0; j158 < 4; j158 = j158 + 1) {
				fRec83_tmp[j158] = fRec83_perm[j158];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec136[i] = ((fZec135[i] > fRec83[i - 1]) ? fConst13 : fConst12);
				fRec83[i] = fZec135[i] * (1.0f - fZec136[i]) + fRec83[i - 1] * fZec136[i];
			}
			/* Post code */
			for (int j159 = 0; j159 < 4; j159 = j159 + 1) {
				fRec83_perm[j159] = fRec83_tmp[vsize + j159];
			}
			/* Vectorizable loop 151 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec139[i] = 2e+01f * std::log10(std::max<float>(1.1754944e-38f, fRec84[i]));
			}
			/* Recursive loop 152 */
			/* Pre code */
			for (int j104 = 0; j104 < 4; j104 = j104 + 1) {
				fRec51_tmp[j104] = fRec51_perm[j104];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec51[i] = fConst14 * fRec51[i - 1] - fConst15 * std::max<float>(2e+01f * std::log10(std::max<float>(1.1754944e-38f, fRec52[i])) + 6.0f, 0.0f);
			}
			/* Post code */
			for (int j105 = 0; j105 < 4; j105 = j105 + 1) {
				fRec51_perm[j105] = fRec51_tmp[vsize + j105];
			}
			/* Vectorizable loop 153 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec101[i] = (fZec97[i] > fZec99[i]) + (fZec97[i] > fZec100[i]);
			}
			/* Vectorizable loop 154 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec102[i] = fZec97[i] + fZec98[i];
			}
			/* Vectorizable loop 155 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec103[i] = 1.0f - 1.0f / (19.0f * fRec54[i] + 1.0f);
			}
			/* Recursive loop 156 */
			/* Pre code */
			for (int j160 = 0; j160 < 4; j160 = j160 + 1) {
				fRec82_tmp[j160] = fRec82_perm[j160];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec82[i] = fConst14 * fRec82[i - 1] - fConst15 * std::max<float>(2e+01f * std::log10(std::max<float>(1.1754944e-38f, fRec83[i])) + 6.0f, 0.0f);
			}
			/* Post code */
			for (int j161 = 0; j161 < 4; j161 = j161 + 1) {
				fRec82_perm[j161] = fRec82_tmp[vsize + j161];
			}
			/* Vectorizable loop 157 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec140[i] = (fZec139[i] > fZec99[i]) + (fZec139[i] > fZec100[i]);
			}
			/* Vectorizable loop 158 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec141[i] = fZec98[i] + fZec139[i];
			}
			/* Vectorizable loop 159 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(fSlow26 * float(input0[i]) + fSlow25 * fZec92[i] * std::pow(1e+01f, 0.05f * fRec51[i]) * std::pow(1e+01f, -(0.05f * std::max<float>(0.0f, ((iZec101[i] == 0) ? 0.0f : ((iZec101[i] == 1) ? 0.16666667f * Macalla_faustpower2_f(fZec102[i] + 1.5f) : fZec102[i]))) * fZec103[i])));
			}
			/* Vectorizable loop 160 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(fSlow26 * float(input1[i]) + fSlow25 * fZec134[i] * std::pow(1e+01f, 0.05f * fRec82[i]) * std::pow(1e+01f, -(0.05f * fZec103[i] * std::max<float>(0.0f, ((iZec140[i] == 0) ? 0.0f : ((iZec140[i] == 1) ? 0.16666667f * Macalla_faustpower2_f(fZec141[i] + 1.5f) : fZec141[i]))))));
			}
		}
	}

};

    // END-FAUSTDSP

#endif
