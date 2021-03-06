/*
 * $Revision: 2564 $
 *
 * last checkin:
 *   $Author: gutwenger $
 *   $Date: 2012-07-07 00:03:48 +0200 (Sa, 07. Jul 2012) $
 ***************************************************************/

/** \file
 * \brief Declaration of class UniformGrid.
 *
 * This is a class implementing a 2-dimensional Hash array.
 * It uses templates for the keys and the data of the objects
 * stored in it.
 *
 * \author Rene Weiskircher
 *
 * \par License:
 * This file is part of the Open Graph Drawing Framework (OGDF).
 *
 * \par
 * Copyright (C)<br>
 * See README.txt in the root directory of the OGDF installation for details.
 *
 * \par
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 or 3 as published by the Free Software Foundation;
 * see the file LICENSE.txt included in the packaging of this file
 * for details.
 *
 * \par
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * \par
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * \see  http://www.gnu.org/copyleft/gpl.html
 ***************************************************************/


#ifdef _MSC_VER
#pragma once
#endif

#ifndef OGDF_TSAUNIFORMGRID_H
#define OGDF_TSAUNIFORMGRID_H

#include<ogdf/AccelerationStructure.h>
#include<ogdf/basic/geometry.h>
#include<ogdf/basic/SList.h>
#include<ogdf/basic/Array2D.h>
#include<ogdf/basic/GraphAttributes.h>
#include<ogdf/basic/HashArray2D.h>
#include<ogdf/internal/energybased/IntersectionRectangle.h>
#include<limits.h>

namespace ogdf {


    class TSAUniformGrid : public AccelerationStructure{
    public:
        TSAUniformGrid(const GraphAttributes &);
        //This constructor takes an GraphAttributes and computes a grid for the given
        //layout.
        TSAUniformGrid(const GraphAttributes &,const node, const DPoint&);
        //This constructor gets the current layout, the node that may be
        //moved and its new position and computes the data for the
        //modified layout.

        virtual void updateNodePosition(const node v, const DPoint& newPos);
        virtual void possibleCrossingEdges(const DPoint& sPos, const DPoint& tPos, List<edge>& result) const;
        virtual void nearNodes(const DPoint &pos, const double maxDist, List<node>& nodes) const;

    private:
        void ModifiedBresenham(const IPoint &, const IPoint &, SList<IPoint> &) const;
        //This takes two DPoints with and computes a list of points
        //that are the lower left corners of the cells that may possibly contain points
        //of the straight line segment connecting the two points
        void DoubleModifiedBresenham(const DPoint &, const DPoint &, SList<IPoint> &) const;
        //this function computes the grid coordinate of a point that depends on the
        //coordiantes of the point, the lower left corner of the bounding rectangle
        //and the size of a cell
        IPoint computeGridPoint(const DPoint &dp) const {
            double x = floor(dp.m_x/m_CellSize);
            OGDF_ASSERT(isInt(x));
            double y = floor(dp.m_y/m_CellSize);
            OGDF_ASSERT(isInt(y));
            return IPoint(int(x),int(y));
        }
        //computes for a grid point the corresponding DPoint
        DPoint computeRealPoint(const IPoint &ip) const {
            DPoint p;
            p.m_x = ip.m_x*m_CellSize;
            p.m_y = ip.m_y*m_CellSize;
            return p;
        }
        //checks if a double number is an integer
        bool isInt(double d) const {
            if(d - floor(d) > 0) return false;
            if(d < INT_MIN || d > INT_MAX) return false;
            return true;
        }

        bool newGridNecessary(const node v, const DPoint& p) {
            bool resize = false;
            IntersectionRectangle ir;
            computeGridGeometry(v,p,ir);
            double l = max(ir.width(),ir.height());
            l/=m_edgeMultiplier*(m_graph).numberOfEdges();
            if(l <= m_CellSize/2.0 || l >= m_CellSize*2.0) resize = true;
            return resize;
        }

        //computes the geometry of the grid if the node is moved
        //to the position given by the point
        void computeGridGeometry(const node, const DPoint&, IntersectionRectangle&) const;

        void initGrid();

#ifdef OGDF_DEBUG
        void markCells(SList<IPoint> &, Array2D<bool> &) const;
        bool crossesCell(IPoint, IPoint, const IPoint &) const;
        bool crossesCell(DPoint, DPoint, const IPoint &) const;
        void checkBresenham(DPoint, DPoint) const;
        void checkBresenham(IPoint, IPoint) const;
        bool intervalIntersect(double,double,double,double) const;
        friend ostream& operator<<(ostream &,const TSAUniformGrid&);
        int m_maxEdgesPerCell;
        double m_time;
#endif
        const GraphAttributes &m_layout; //the layout
        const Graph &m_graph;
        HashArray2D<int,int,List<edge> > m_grid; //stores for each grid cell
        //the Array of edges that cross that cell
        EdgeArray<List<IPoint> > m_cells; //Contains for each edge the
        //list of cells it crosses
        double m_CellSize; //Sidelength of one cell
        const static double m_epsilon; //tolerance fo double computation
        const static double m_edgeMultiplier; //this controls the gridsize

        TSAUniformGrid& operator=(const TSAUniformGrid& ug);
    };
#ifdef OGDF_DEBUG
ostream &operator<<(ostream &, const IPoint &);
#endif
} //namespace
#endif
