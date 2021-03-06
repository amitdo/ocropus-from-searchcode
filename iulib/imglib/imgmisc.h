// -*- C++ -*-

#ifndef h_imgmisc__
#define h_imgmisc__

// Copyright 2006 Deutsches Forschungszentrum fuer Kuenstliche Intelligenz
// or its licensors, as applicable.
// Copyright 1995-2005 Thomas M. Breuel
//
// You may not use this file except under the terms of the accompanying license.
//
// Licensed under the Apache License, Version 2.0 (the "License");you
// may not use this file except in compliance with the License. You may
// obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Project: iulib -- image understanding library
// File: imgmisc.h
// Purpose: interface to corresponding .cc file
// Responsible: tmb
// Reviewer:
// Primary Repository:
// Web Sites: www.iupr.org, www.dfki.de
#include "colib/colib.h"

namespace iulib {
    using namespace colib;

    void valleys(intarray &locations, floatarray &v, int minsize=0,
            int maxsize=1<<30, float sigma=0.0);
    void peaks(intarray &locations, floatarray &v, int minsize=0,
            int maxsize=1<<30, float sigma=0.0);
    void hist(floatarray &hist, bytearray &image);

    template <class T>
    inline void split_rgb(narray<T> &r,narray<T> &g,narray<T> &b,narray<T> &rgb) {
        CHECK_ARG(rgb.rank()==3);
        int w = rgb.dim(0), h = rgb.dim(1);
        r.resize(w,h);
        g.resize(w,h);
        b.resize(w,h);
        for(int i=0;i<w;i++) for(int j=0;j<h;j++) {
            r(i,j) = rgb(i,j,0);
            g(i,j) = rgb(i,j,1);
            b(i,j) = rgb(i,j,2);
        }
    }

    template <class T>
    inline void combine_rgb(narray<T> &rgb,narray<T> &r,narray<T> &g,narray<T> &b) {
        CHECK_ARG(r.rank()==2 && g.rank()==2 && b.rank()==2);
        CHECK_ARG(samedims(r,g) && samedims(g,b));
        int w = r.dim(0), h = r.dim(1);
        rgb.resize(w,h,3);
        for(int i=0;i<w;i++) for(int j=0;j<h;j++) {
            rgb(i,j,0) = r(i,j);
            rgb(i,j,1) = g(i,j);
            rgb(i,j,2) = b(i,j);
        }
    }

    inline void unpack_rgb(bytearray &r,bytearray &g,bytearray &b,intarray &rgb) {
        CHECK_ARG(rgb.rank()==2);
        int w = rgb.dim(0), h = rgb.dim(1);
        r.resize(w,h);
        g.resize(w,h);
        b.resize(w,h);
        for(int i=0;i<w;i++) for(int j=0;j<h;j++) {
            r(i,j) = (rgb(i,j)>>16);
            g(i,j) = (rgb(i,j)>>8);
            b(i,j) = rgb(i,j);
        }
    }

    inline void pack_rgb(intarray &rgb,bytearray &r,bytearray &g,bytearray &b) {
        CHECK_ARG(r.rank()==2 && g.rank()==2 && b.rank()==2);
        CHECK_ARG(samedims(r,g) && samedims(g,b));
        int w = r.dim(0), h = r.dim(1);
        rgb.resize(w,h);
        for(int i=0;i<w;i++) for(int j=0;j<h;j++) {
            rgb(i,j) = ((r(i,j)<<16) | (g(i,j)<<8) | b(i,j));
        }
    }

    template<class T>
    inline void fill_rect(narray<T> &out,int x0,int y0,int x1,int y1,T value) {
        for(int i=x0;i<x1;i++)
            for(int j=y0;j<y1;j++)
                out(i,j) = value;
    }

    template<class T>
    inline void fill_rect(narray<T> &out,rectangle r,T value) {
        fill_rect(out,r.x0,r.y0,r.x1,r.y1,value);
    }

    template<class T>
    inline void math2raster(narray<T> &out, narray<T> &in) {
        int w = in.dim(0), h = in.dim(1);
        out.resize(h, w);
        for (int x=0; x<w; x++) {
            for (int y=0; y<h; y++)
                out(h-y-1, x)=in(x, y);
        }
    }

    template<class T>
    inline void math2raster(narray<T> &out) {
        narray<T> temp;
        math2raster(temp,out);
        out.move(temp);
    }

    template<class T>
    inline void raster2math(narray<T> &out, narray<T> &in) {
        int w = in.dim(0), h = in.dim(1);
        out.resize(h, w);
        for (int x=0; x<w; x++) {
            for (int y=0; y<h; y++)
                out(h-y-1, x)=in(x, y);
        }
    }

    template<class T>
    inline void raster2math(narray<T> &out) {
        narray<T> temp;
        raster2math(temp,out);
        out.move(temp);
    }

    template<class T>
    inline void crop(narray<T> &result, narray<T> &source,
            int x, int y, int w, int h) {
        result.resize(w, h);
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++)
                result(i, j) = source(x + i, y + j);
        }
    }

    template<class T>
    inline void crop(narray<T> &result, narray<T> &source,
            rectangle r) {
        crop<T>(result, source, r.x0, r.y0, r.x1 - r.x0, r.y1 - r.y0);
    }

    template<class T>
    inline void crop(narray<T> &a, rectangle box) {
        narray<T> t;
        extract_subimage(t, a, box.x0, box.y0, box.x1, box.y1);
        move(a, t);
    }

    template<class T>
    inline void transpose(narray<T> &a) {
        narray<T> t;
        t.resize(a.dim(1), a.dim(0));
        for (int x = 0; x < a.dim(0); x++) {
            for (int y = 0; y < a.dim(1); y++)
                t(y, x) = a(x,y);
        }
        move(a, t);
    }

    template<class T>
    inline void replace_values(narray<T> &a, T from, T to) {
        for (int i = 0; i < a.length1d(); i++) {
            if (a.at1d(i) == from)
                a.at1d(i) = to;
        }
    }

    void binarize_by_threshold(bytearray &image);
    void check_page_segmentation(intarray &pseg);
    void make_page_segmentation_black(intarray &a);
    void make_page_segmentation_white(intarray &a);
    void check_line_segmentation(intarray &cseg);
    void make_line_segmentation_black(intarray &a);
    void make_line_segmentation_white(intarray &a);
}

#endif
