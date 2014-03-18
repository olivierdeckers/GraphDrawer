/*
 * $Revision: 2616 $
 *
 * last checkin:
 *   $Author: gutwenger $
 *   $Date: 2012-07-16 15:34:43 +0200 (Mo, 16. Jul 2012) $
 ***************************************************************/

/** \file
 * \brief Implementation of class UniformGrid
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

#include<ogdf/TSAUniformGrid.h>
#include<float.h>

namespace ogdf {
    const double TSAUniformGrid::m_epsilon = 0.000001;
    const double TSAUniformGrid::m_edgeMultiplier = 1.0;

//	int UniformGrid::constructorcounter = 0;

    void TSAUniformGrid::ModifiedBresenham(
        const IPoint &p1,
        const IPoint &p2,
        SList<IPoint> &crossedCells) const{
        crossedCells.clear();

        int Ax = p1.m_x;
        int Ay = p1.m_y;
        int Bx = p2.m_x;
        int By = p2.m_y;
        //------------------------------------------------------------------------
        // INITIALIZE THE COMPONENTS OF THE ALGORITHM THAT ARE NOT AFFECTED BY THE
        // SLOPE OR DIRECTION OF THE LINE
        //------------------------------------------------------------------------
        int dX = abs(Bx-Ax);	// store the change in X and Y of the line endpoints
        int dY = abs(By-Ay);

        //------------------------------------------------------------------------
        // DETERMINE "DIRECTIONS" TO INCREMENT X AND Y (REGARDLESS OF DECISION)
        //------------------------------------------------------------------------
        int Xincr, Yincr,Xoffset,Yoffset;
        if (Ax > Bx) { Xincr=-1; Xoffset = -1;} else { Xincr=1; Xoffset = 0;}	// which direction in X?
        if (Ay > By) { Yincr=-1; Yoffset = -1;} else { Yincr=1; Yoffset = 0;}	// which direction in Y?
        // the offsets are necessary because we always want the cell left and below
        // the point were bresenham wants to draw it

        //------------------------------------------------------------------------
        // DETERMINE INDEPENDENT VARIABLE (ONE THAT ALWAYS INCREMENTS BY 1 (OR -1) )
        // AND INITIATE APPROPRIATE LINE DRAWING ROUTINE (BASED ON FIRST OCTANT
        // ALWAYS). THE X AND Y'S MAY BE FLIPPED IF Y IS THE INDEPENDENT VARIABLE.
        //------------------------------------------------------------------------
        if (dX >= dY)	// if X is the independent variable
        {
            int dPr 	= dY<<1;  // amount to increment decision if right is chosen (always)
            int dPru 	= dPr - (dX<<1);   // amount to increment decision if up is chosen
            int P 		= dPr - dX;  // decision variable start value
            int secondY = Ay+Yincr; //Y-coordinate of secondary point
            int testval = P;	//if P is equal to testval, the the next point is drawn exactly
                                // on the segment. If P is smaller than testval, it is below the
                                //segment


            for (; dX>=0; dX--)  // process each point in the line one at a time (just use dX)
            {
                crossedCells.pushBack(IPoint(Ax+Xoffset,Ay+Yoffset));//add the primary cell
                crossedCells.pushBack(IPoint(Ax+Xoffset,secondY+Yoffset));//add the secondary cell
                if (P > 0)          // is the pixel going right AND up?
                {
                    Ax+=Xincr;	       // increment independent variable
                    Ay+=Yincr;         // increment dependent variable
                    P+=dPru;           // increment decision (for up)
                }
                else                     // is the pixel just going right?
                {
                    Ax+=Xincr;         // increment independent variable
                    P+=dPr;            // increment decision (for right)
                }
                if(P - testval < 0) //primary cell above the line
                    secondY = Ay-Yincr;
                else secondY = Ay+Yincr;//primary cell below the line
            }
        }
        else              // if Y is the independent variable
        {
            int dPr 	= dX<<1;    // amount to increment decision if right is chosen (always)
            int dPru 	= dPr - (dY<<1);   // amount to increment decision if up is chosen
            int P 		= dPr - dY;  // decision variable start value
            int testval = P; // substracting this from P tells us if the cell is drawn left or
                            // right from the actual segment
            int secondX = Ax+Xincr; //X-Coordinate of secondary cell

            for (; dY>=0; dY--)            // process each point in the line one at a time (just use dY)
            {
                crossedCells.pushBack(IPoint(Ax+Xoffset,Ay+Yoffset));// add the primary cell
                crossedCells.pushBack(IPoint(secondX+Xoffset,Ay+Yoffset));// add the secondary cell
                if (P > 0)               // is the pixel going up AND right?
                {
                    Ax+=Xincr;         // increment dependent variable
                    Ay+=Yincr;         // increment independent variable
                    P+=dPru;           // increment decision (for up)
                }
                else                     // is the pixel just going up?
                {
                    Ay+=Yincr;         // increment independent variable
                    P+=dPr;            // increment decision (for right)
                }
                if(P - testval < 0) //primary cell left of the line
                    secondX = Ax-Xincr;
                else secondX = Ax+Xincr;//primary cell right of the line
            }
        }

    }

    void TSAUniformGrid::DoubleModifiedBresenham(
        const DPoint &p1,
        const DPoint &p2,
        SList<IPoint> &crossedCells) const{
        crossedCells.clear();
        //------------------------------------------------------------------------
        // INITIALIZE THE COMPONENTS OF THE ALGORITHM THAT ARE NOT AFFECTED BY THE
        // SLOPE OR DIRECTION OF THE LINE
        //------------------------------------------------------------------------
        double dX = fabs(p2.m_x-p1.m_x);	// store the change in X and Y of the line endpoints
        double dY = fabs(p1.m_y-p2.m_y);


        //------------------------------------------------------------------------
        // DETERMINE INDEPENDENT VARIABLE (ONE THAT ALWAYS INCREMENTS BY 1 (OR -1) )
        // AND INITIATE APPROPRIATE LINE DRAWING ROUTINE (BASED ON FIRST OCTANT
        // ALWAYS). THE X AND Y'S MAY BE FLIPPED IF Y IS THE INDEPENDENT VARIABLE.
        //------------------------------------------------------------------------
        if (dX >= dY)	// if X is the independent variable
        {
            DPoint left,right;
            if(p1.m_x > p2.m_x) {
                left = p2;
                right = p1;
            }
            else {
                left = p1;
                right= p2;
            }
            //Now we determine the coordinates of the start cell
            //and the end cell
            IPoint start(computeGridPoint(left));
            if(p1 == p2) {
                crossedCells.pushBack(start);
                return;
            }
            IPoint end(computeGridPoint(right));
            //Since computeGridPoint rounds down, this gives us the point p1 and
            //below each of the points. This is the address of the cell that contains
            //the point
            //int Yincr = 1;
            //if(left.m_y > right.m_y) Yincr = -1;
            double slope = (right.m_y-left.m_y)/(right.m_x-left.m_x);
            double c = left.m_y-slope*left.m_x;
            OGDF_ASSERT(fabs(slope*right.m_x+c - right.m_y) < m_epsilon);
            int endX = end.m_x+1;
            double dYincr = slope * m_CellSize;
            double OldYPos = slope*start.m_x*m_CellSize+c;
            int oldY = (int)floor(OldYPos/m_CellSize);
            for(int i = start.m_x; i <= endX; i++) {
                crossedCells.pushBack(IPoint(i,oldY));
                double newY = OldYPos + dYincr;
                OGDF_ASSERT(newY - ((i+1)*m_CellSize*slope+c) < m_epsilon)
                int newCellY = (int)floor(newY/m_CellSize);
                if(newCellY != oldY) {
                    oldY = newCellY;
                    crossedCells.pushBack(IPoint(i,oldY));
                }
                OldYPos = newY;
            }
        }
        else              // if Y is the independent variable
        {
            DPoint bottom,top;
            if(p1.m_y > p2.m_y) {
                bottom = p2;
                top = p1;
            }
            else {
                bottom = p1;
                top = p2;
            }
            IPoint start(computeGridPoint(bottom));
            IPoint end(computeGridPoint(top));
            //int Xincr = 1;
            //if(bottom.m_x > top.m_x) Xincr = -1;
            double slope = (top.m_x-bottom.m_x)/(top.m_y-bottom.m_y);
            double c = bottom.m_x-slope*bottom.m_y;
            OGDF_ASSERT(fabs(slope*top.m_y+c - top.m_x) < m_epsilon);
            int endY = end.m_y+1;
            double dXincr = slope * m_CellSize;
            double OldXPos = slope*start.m_y*m_CellSize+c;
            int oldX = (int)floor(OldXPos/m_CellSize);
            for(int i = start.m_y; i <= endY; i++) {
                crossedCells.pushBack(IPoint(oldX,i));
                double newX = OldXPos + dXincr;
                OGDF_ASSERT(newX - ((i+1)*m_CellSize*slope+c) < m_epsilon)
                int newCellX = (int)floor(newX/m_CellSize);
                if(newCellX != oldX) {
                    oldX = newCellX;
                    crossedCells.pushBack(IPoint(oldX,i));
                }
                OldXPos = newX;
            }
        }

    }
//constructor for computing the grid and the crossings from scratch for the
//layout given by AG
TSAUniformGrid::TSAUniformGrid(const GraphAttributes &AG) :
    m_layout(AG),
    m_graph(AG.constGraph()),
    m_cells(m_graph),
    m_CellSize(0.0)
{
    //cout<<"New grid \n";
    node v = m_graph.firstNode();
    DPoint pos(m_layout.x(v),m_layout.y(v));
#ifdef OGDF_DEBUG
    m_maxEdgesPerCell = 0;
    usedTime(m_time);
#endif
    IntersectionRectangle ir;
    computeGridGeometry(v,pos,ir);
    double maxLength = max(ir.height(),ir.width());
    m_CellSize = maxLength/(m_edgeMultiplier*(m_graph).numberOfEdges());

    initGrid();
#ifdef OGDF_DEBUG
    m_time = usedTime(m_time);
#endif
}

//constructor for computing the grid and the crossings from scratch for
//the given layout where node v is moved to newPos
TSAUniformGrid::TSAUniformGrid(
    const GraphAttributes &AG,
    const node v,
    const DPoint& newPos)
:
    m_layout(AG),
    m_graph(AG.constGraph()),
    m_cells(m_graph),
    m_CellSize(0.0)
{
#ifdef OGDF_DEBUG
    m_maxEdgesPerCell = 0;
    usedTime(m_time);
#endif
    IntersectionRectangle ir;
    computeGridGeometry(v,newPos,ir);
    double maxLength = max(ir.height(),ir.width());
    m_CellSize = maxLength/(m_edgeMultiplier*(m_graph).numberOfEdges());

    initGrid();
#ifdef OGDF_DEBUG
    m_time = usedTime(m_time);
#endif
}

void TSAUniformGrid::updateNodePosition(const node v, const DPoint &newPos)
{
    if(newGridNecessary(v, newPos)) {
        m_cells = EdgeArray<List<IPoint>>(m_graph);

        IntersectionRectangle ir;
        computeGridGeometry(v,newPos,ir);
        double maxLength = max(ir.height(),ir.width());
        m_CellSize = maxLength/(m_edgeMultiplier*(m_graph).numberOfEdges());

        initGrid();
    }
    else {
        //compute the list of edge incident to v
        List<edge> incident;
        m_graph.adjEdges(v,incident);

        ListIterator<edge> it1;
        for(it1 = incident.begin(); it1.valid(); ++it1) {
            edge& e = *it1;
            List<IPoint>& cells = m_cells[e];
            //delete e from all its cells
            while(!cells.empty()) {
                IPoint p = cells.popFrontRet();
                List<edge>& eList = m_grid(p.m_x,p.m_y);
                ListIterator<edge> it2 = eList.begin();
                while(*it2 != e) ++it2;
                eList.del(it2);
            }
        }

        //now we compute all the remaining data of the class in one loop
        //going through all edges and storing them in the grid.
        ListConstIterator<edge> it;
        for(it = incident.begin(); it.valid(); ++it) {
            const edge& e = *it;
            SList<IPoint> crossedCells;
            DPoint sPos,tPos;
            const node& s = e->source();
            if(s != v) sPos = DPoint(m_layout.x(s),m_layout.y(s));
            else sPos = newPos;
            const node& t = e->target();
            if(t != v) tPos = DPoint(m_layout.x(t),m_layout.y(t));
            else tPos = newPos;
            DoubleModifiedBresenham(sPos,tPos,crossedCells);
            SListConstIterator<IPoint> it1;
            for(it1 = crossedCells.begin(); it1.valid(); ++it1) {
                const IPoint& p = *it1;
                (m_cells[e]).pushBack(p);
                List<edge>& edgeList = m_grid(p.m_x,p.m_y);

                //now we insert the new edge into the list and store the position
                //returned by pushBack in the corresponding list of m_storedIn
                edgeList.pushBack(e);
#ifdef OGDF_DEBUG
                if(m_maxEdgesPerCell < edgeList.size())
                    m_maxEdgesPerCell = edgeList.size();
#endif
            }
        }
    }
}

void TSAUniformGrid::possibleCrossingEdges(const DPoint &sPos, const DPoint &tPos, List<edge> &result) const
{
    SList<IPoint> crossedCells;
    DoubleModifiedBresenham(sPos, tPos, crossedCells);

    Hashing<edge,bool> edgesEncountered;
    SListConstIterator<IPoint> it;
    for(it = crossedCells.begin(); it.valid(); it++) {
        IPoint p = *it;
        List<edge> edges = m_grid(p.m_x, p.m_y);
        ListIterator<edge> it2;
        for(it2 = edges.begin(); it2.valid(); it2++) {
            if(!edgesEncountered.member(*it2)) {
                edgesEncountered.insert(*it2, true);
                result.pushBack(*it2);
            }
        }
    }
}

void TSAUniformGrid::nearNodes(const DPoint &pos, const double maxDist, List<node> &nodes) const
{
    //TODO
}

void TSAUniformGrid::computeGridGeometry(
    const node moved,
    const DPoint& newPos,
    IntersectionRectangle& ir) const
{
    //first we compute the resolution and size of the grid
    double MinX = DBL_MAX, MinY = DBL_MAX, MaxX =DBL_MIN, MaxY = DBL_MIN;
    //find lower left and upper right vertex
    node v;
    forall_nodes(v,m_graph) {
        double x = 0.0, y = 0.0;
        if(v != moved) {// v is the node that was moved
            x = m_layout.x(v);
            y = m_layout.y(v);
        }
        else {// v is not the moved node
            x = newPos.m_x;
            y = newPos.m_y;
        }
        if(x < MinX) MinX = x;
        if(x > MaxX) MaxX = x;
        if(y < MinY) MinY = y;
        if(y > MaxY) MaxY = y;
    }
    ir = IntersectionRectangle(MinX,MinY,MaxX,MaxY);
}

void TSAUniformGrid::initGrid()
{
    List<edge> edges;
    m_graph.allEdges(edges);

    ListConstIterator<edge> it;
    for(it = edges.begin(); it.valid(); ++it) {
        const edge& e = *it;
        SList<IPoint> crossedCells;
        DPoint sPos,tPos;
        const node& s = e->source();
        sPos = DPoint(m_layout.x(s),m_layout.y(s));
        const node& t = e->target();
        tPos = DPoint(m_layout.x(t),m_layout.y(t));
        DoubleModifiedBresenham(sPos,tPos,crossedCells);
        SListConstIterator<IPoint> it1;
        for(it1 = crossedCells.begin(); it1.valid(); ++it1) {
            const IPoint& p = *it1;
            (m_cells[e]).pushBack(p);
            List<edge>& edgeList = m_grid(p.m_x,p.m_y);
            edgeList.pushBack(e);
        }
    }
}


#ifdef OGDF_DEBUG

void TSAUniformGrid::markCells(SList<IPoint> &result, Array2D<bool> &cells) const {
        while(!result.empty()) {
            IPoint p = result.popFrontRet();
            if(cells.low1() <= p.m_x && cells.high1() >= p.m_x
                && cells.low2() <= p.m_y && cells.high2() >= p.m_y)
                cells(p.m_x,p.m_y) = true;
        }
}


void TSAUniformGrid::checkBresenham(DPoint p1, DPoint p2) const
{
    int crossed = 0;
    DPoint bottomleft(min(p1.m_x,p2.m_x),min(p1.m_y,p2.m_y));
    DPoint topright(max(max(p1.m_x,p2.m_x),bottomleft.m_x+1.0),
        max(max(p1.m_y,p2.m_y),bottomleft.m_y+1.0));
    IPoint ibl(computeGridPoint(bottomleft));
    IPoint itr(computeGridPoint(topright));
    Array2D<bool> cells(ibl.m_x,itr.m_x+1,ibl.m_y,itr.m_y+1,false);
    SList<IPoint> result;
    DoubleModifiedBresenham(p1,p2,result);
    cout << "\nList computed by Bresenham:\n";

    for(SListIterator<IPoint> it = result.begin(); it.valid(); ++it) {
        cout << computeRealPoint(*it) << " ";
    }

    markCells(result,cells);
    cout << "\nCrossed cells:\n";
    if(p1.m_x == p2.m_x) { //vertical segment
        int cellXcoord = (int)floor(p1.m_x/m_CellSize);
        double b = floor(min(p1.m_y,p2.m_y)/m_CellSize);
        double t = ceil(max(p1.m_y,p2.m_y)/m_CellSize);
        OGDF_ASSERT(isInt(b) && isInt(t));
        int intT = (int)t;
        for(int i = int(b); i < intT; i++) {
            crossed++;
            IPoint p(cellXcoord,i);
            cout << computeRealPoint(p) << " ";
            if(!cells(p.m_x,p.m_y)) {
                cout << "\nCell " << computeRealPoint(p) << " is not marked!";
                exit(1);
            }
        }
    }
    else {
        if(p1.m_y == p2.m_y) { //horizontal segment
            double tmp = floor(p1.m_y/m_CellSize);
            assert(isInt(tmp));
            int cellYcoord = (int)tmp;
            double l = floor(min(p1.m_x,p2.m_x)/m_CellSize);
            double r = ceil(max(p1.m_x,p2.m_x)/m_CellSize);
            assert(isInt(l) && isInt(r));
            int intR = (int)r;
            for(int i = int(l); i < intR; i++) {
                crossed++;
                IPoint p(i,cellYcoord);
                cout << computeRealPoint(p) << " ";
                if(!cells(p.m_x,p.m_y)) {
                    cout << "\nCell " << computeRealPoint(p) << " is not marked!";
                    exit(1);
                }
            }
        }
        else {
            for(int i = cells.low1(); i <= cells.high1(); i++) {
                for(int j = cells.low2(); j <= cells.high2(); j++) {
                    IPoint p(i,j);
                    if(crossesCell(p1,p2,p)) {
                        crossed++;
                        cout << computeRealPoint(p) << " ";
                        if(!cells(p.m_x,p.m_y)) {
                            cout << "\n Cell " << computeRealPoint(p) << " is not marked!";
                            exit(1);
                        }
                    }
                }
            }

        }
    }
    if(crossed < max(fabs(p1.m_x-p2.m_x)/m_CellSize,fabs(p1.m_y-p2.m_y)/m_CellSize)) {
        cout << "\nNot enough crossed cells for " << p1 << " " << p2 << "\n";
        exit(1);
    }
    cout << "\n";

}


void TSAUniformGrid::checkBresenham(IPoint p1, IPoint p2) const
{
    int crossed = 0;
    int left = min(p1.m_x,p2.m_x)-1;
    int right = max(max(p1.m_x,p2.m_x),left+1);
    int bottom = min(p1.m_y,p2.m_y)-1;
    int top = max(max(p1.m_y,p2.m_y),bottom+1);
    Array2D<bool> cells(left,right,bottom,top,false);
    SList<IPoint> result;
    ModifiedBresenham(p1,p2,result);
    cout << "\nList computed by Bresenham:\n" << result;
    markCells(result,cells);
    cout << "\nCrossed cells:\n";
    if(p1.m_x == p2.m_x) { //vertical segment
        for(int i = min(p1.m_y,p2.m_y); i < max(p1.m_y,p2.m_y); i++) {
            crossed++;
            IPoint p(p1.m_x,i);
            cout << p << " ";
            if(!cells(p.m_x,p.m_y)) {
                cout << "\nCell " << p << " is not marked!";
                exit(1);
            }
        }
    }
    else {
        if(p1.m_y == p2.m_y) { //horizontal segment
            for(int i = min(p1.m_x,p2.m_x); i < max(p1.m_x,p2.m_x); i++) {
                crossed++;
                IPoint p(i,p1.m_y);
                cout << p << " ";
                if(!cells(i,p1.m_y)) {
                    cout << "\nCell " << p <<" is not marked!";
                    exit(1);
                }
            }
        }
        else {
            for(int i = cells.low1(); i <= cells.high1(); i++) {
                for(int j = cells.low2(); j <= cells.high2(); j++) {
                    IPoint p(i,j);
                    if(crossesCell(p1,p2,p)) {
                        crossed++;
                        cout << p << " ";
                        if(!cells(p.m_x,p.m_y)) {
                            cout << "\n Cell " << p << " is not marked!";
                            exit(1);
                        }
                    }
                }
            }

        }
    }
    if(crossed < max(abs(p1.m_x-p2.m_x),abs(p1.m_y-p2.m_y))) {
        cout << "\nNot enough crossed cells for " << p1 << " " << p2 << "\n";
        exit(1);
    }
    cout << "\n";

}


//the upper and left boundary does not belong to a cell
bool TSAUniformGrid::crossesCell(
    IPoint A,
    IPoint B,
    const IPoint &CellAdr) const
{
    bool crosses = false;
    if(A.m_x == B.m_x) {//line segment is vertical
        if(A.m_x >= CellAdr.m_x && A.m_x < CellAdr.m_x+1) {
            if(intervalIntersect(A.m_y,B.m_y,CellAdr.m_y,CellAdr.m_y+1))
                crosses = true;
        }
    }
    else {//line segment not vertical
        if(A.m_x > B.m_x) swap(A,B);
        double m = double(B.m_y-A.m_y)/double(B.m_x-A.m_x);
        double c = A.m_y-A.m_x*m;
        double y1 = m*CellAdr.m_x + c;
        double y2 = m*(CellAdr.m_x+1)+c;
        crosses = intervalIntersect(A.m_x,B.m_x,CellAdr.m_x,CellAdr.m_x+1);
        crosses = crosses && intervalIntersect(y1,y2,CellAdr.m_y,CellAdr.m_y+1);
    }
    return crosses;
}


//the upper and left boundary does not belong to a cell
bool TSAUniformGrid::crossesCell(
    DPoint A,
    DPoint B,
    const IPoint &CellAdr) const
{
    bool crosses = false;
    double xLowCell = CellAdr.m_x * m_CellSize;
    double xHighCell = xLowCell + m_CellSize;
    double yLowCell = CellAdr.m_y * m_CellSize;
    double yHighCell = yLowCell + m_CellSize;
    if(A.m_x == B.m_x) {//line segment is vertical
        if(A.m_x >= xLowCell && A.m_x < xHighCell) {
            if(intervalIntersect(A.m_y,B.m_y,yLowCell,yHighCell))
                crosses = true;
        }
    }
    else {//line segment not vertical
        if(A.m_x > B.m_x) swap(A,B);
        double m = (B.m_y-A.m_y)/(B.m_x-A.m_x);
        double c = A.m_y-A.m_x*m;
        double y1 = m * xLowCell + c;
        double y2 = m * xHighCell + c;
        crosses = intervalIntersect(A.m_x,B.m_x,xLowCell,xHighCell);
        crosses = crosses && intervalIntersect(min(A.m_y,B.m_y),max(A.m_y,B.m_y),
            yLowCell,yHighCell);
        crosses = crosses && intervalIntersect(y1,y2,yLowCell,yHighCell);
    }
    return crosses;
}


bool TSAUniformGrid::intervalIntersect(
    double a1,
    double a2,
    double cell1,
    double cell2) const
{
    double epsilon = 0.000001;
    bool intersect = true;
    if(min(a1,a2)+epsilon >= max(cell1,cell2) || min(cell1,cell2)+epsilon >= max(a1,a2)) intersect = false;
    return intersect;
}


ostream &operator<<(ostream &out, const TSAUniformGrid &ug)
{
    out << "\nGrid Size: " << ug.m_CellSize;
    out << "\nEpsilon: " << ug.m_epsilon;
    out << "\nEdge Multiplier: " << ug.m_edgeMultiplier;
#ifdef OGDF_DEBUG
    out << "\nMax edges per cell: " << ug.m_maxEdgesPerCell;
    out << "\nConstruction time: " << ug.m_time;
    IntersectionRectangle ir;
    node v = ug.m_graph.firstNode();
    ug.computeGridGeometry(v,DPoint(ug.m_layout.x(v),ug.m_layout.y(v)),ir);
    double l = max(ir.width(),ir.height());
    cout << "\nPreferred Cell Size: " << l/(ug.m_graph.numberOfEdges()*ug.m_edgeMultiplier);
#endif
    return out;
}


#endif
}//namespace
