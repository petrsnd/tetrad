using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Media;

namespace TetraSlide.Win8.Controls
{
    enum Axis
    {
        X,
        Y
    }

    class DragConstraints
    {
        Point DragStart;
        public DragConstraints(Board theboard, 
            Point dragStart,
            Tile dragTile,
            TsWinRT.TsPosition dragPosition, 
            Dictionary<TsWinRT.TsPosition, BoardSpace> targets)
        {
            CurrentDirection = TsWinRT.TsBoardDirection.TsDirInvalid;
            DragStart = dragStart;

            // Init constraints to the least constrained possibilities
            GeneralTransform gt = dragTile.TransformToVisual(theboard);
            Rect source = gt.TransformBounds(new Rect(new Point(0, 0), dragTile.RenderSize));
            
            foreach (KeyValuePair<TsWinRT.TsPosition, BoardSpace> kvp in targets)
            {
                FrameworkElement element = kvp.Value as FrameworkElement;
                gt = element.TransformToVisual(theboard);
                Rect target = gt.TransformBounds(new Rect(new Point(0, 0), element.RenderSize));
                double diffY = target.Y - source.Y;
                double diffX = target.X - source.X;
                switch (GetRelativeDirection(dragPosition, kvp.Key))
                {
                    case TsWinRT.TsBoardDirection.TsDirUp:
                        MinY = MinY < diffY ? MinY : diffY;
                        //MinY -= (dragStart.Y - source.Y);
                        break;
                    case TsWinRT.TsBoardDirection.TsDirDown:
                        double diffDown = target.Y - source.Y;
                        MaxY = MaxY > diffY ? MaxY : diffY;
                        //MaxY += source.Height - dragStart.Y; 
                        break;
                    case TsWinRT.TsBoardDirection.TsDirLeft:
                        MinX = MinX < diffX ? MinX : diffX;
                        //MinX -= (dragStart.X - source.X);
                        break;
                    case TsWinRT.TsBoardDirection.TsDirRight:
                        MaxX = MaxX > diffX ? MaxX : diffX;
                        //MaxX += source.Width - dragStart.X;
                        break;
                    default: break;
                }
            }
        }

        public Point GetDragDifference(Point cur, TranslateTransform tt)
        {
            Point pt = new Point();
            if (DragAxis == Axis.X)
            {
                double xdiff = cur.X - DragStart.X;
                if (xdiff > MinX && xdiff < MaxX)
                {
                    pt.Y = 0;
                    pt.X = xdiff;
                }
                else
                {
                    if (xdiff < MinX)
                        pt.X = MinX;
                    else if (xdiff > MaxX)
                        pt.X = MaxX;
                }
            }
            else
            {
                double ydiff = cur.Y - DragStart.Y;
                if (ydiff > MinY && ydiff < MaxY)
                {
                    pt.X = 0;
                    pt.Y = ydiff;
                }
                else
                {
                    // Create and show an X adorner?
                }
            }
            return pt;
        }

        public void DoDrag(Point cur, TranslateTransform tt)
        {
            if (DragAxis == Axis.X)
            {
                double xdiff = cur.X - DragStart.X;
                if (xdiff > MinX && xdiff < MaxX)
                {
                    tt.Y = 0;
                    tt.X = xdiff;
                }
                else
                {
                    if (xdiff < MinX)
                        tt.X = MinX;
                    else if (xdiff > MaxX)
                        tt.X = MaxX;
                }
            }
            else
            {
                double ydiff = cur.Y - DragStart.Y;
                if (ydiff > MinY && ydiff < MaxY)
                {
                    tt.X = 0;
                    tt.Y = ydiff;
                }
                else
                {
                    // Create and show an X adorner?
                }
            }
        }

        public Double MaxX
        {
            get;
            private set;
        }

        public Double MinX
        {
            get;
            private set;
        }

        public Double MaxY
        {
            get;
            private set;
        }

        public Double MinY
        {
            get;
            private set;
        }

        bool IsValidDirection(TsWinRT.TsBoardDirection dir)
        {
            switch (dir)
            {
                case TsWinRT.TsBoardDirection.TsDirUp:
                    return MinY < 0;
                case TsWinRT.TsBoardDirection.TsDirDown:
                    return MaxY > 0;
                case TsWinRT.TsBoardDirection.TsDirLeft:
                    return MinX < 0;
                case TsWinRT.TsBoardDirection.TsDirRight:
                    return MaxX > 0;
            }
            return false;
        }

        public Axis DragAxis
        {
            get;
            private set;
        }

        public void SetDragAxis(Point cur)
        {
            DragAxis = CalculateAxis(DragStart, cur);
        }

        public TsWinRT.TsBoardDirection CurrentDirection
        {
            get;
            private set;
        }

        public Axis CalculateAxis(Point from, Point to)
        {
            TsWinRT.TsBoardDirection dir = CalculateDragDirection(from, to);
            if( ! IsValidDirection( dir ) )
            {
                return DragAxis;
            }
            CurrentDirection = dir;
            switch (dir)
            {
                case TsWinRT.TsBoardDirection.TsDirUp:
                case TsWinRT.TsBoardDirection.TsDirDown:
                    return Axis.Y;
                case TsWinRT.TsBoardDirection.TsDirLeft:
                case TsWinRT.TsBoardDirection.TsDirRight:
                    return Axis.X; 
            }
            throw new Exception("Invalid spacial dimension vortex!");
        }

        public bool CanMove(TsWinRT.TsBoardDirection dir)
        {
            
            return 
                (DragAxis == Axis.X &&
                 (dir == TsWinRT.TsBoardDirection.TsDirRight ||
                 (dir == TsWinRT.TsBoardDirection.TsDirLeft))) 
                ||
                (DragAxis == Axis.Y &&
                 (dir == TsWinRT.TsBoardDirection.TsDirUp ||
                 (dir == TsWinRT.TsBoardDirection.TsDirDown)));
        }

        public TsWinRT.TsBoardDirection CalculateDragDirection(Point origin, Point dragTo)
        {
            double diffx = dragTo.X - origin.X;
            double diffy = dragTo.Y - origin.Y;
            diffy *= -1;
            double degrees = Math.Round(Math.Atan2(diffy, diffx) * (180 / Math.PI));
            if (degrees < 0)
                degrees = 360 + degrees;
            if (degrees >= 315 || degrees < 45)
                return TsWinRT.TsBoardDirection.TsDirRight;
            else if (degrees >= 45 && degrees < 135)
                return TsWinRT.TsBoardDirection.TsDirUp;
            else if (degrees >= 135 && degrees < 225)
                return TsWinRT.TsBoardDirection.TsDirLeft;
            else return TsWinRT.TsBoardDirection.TsDirDown;
        }

        TsWinRT.TsBoardDirection GetRelativeDirection(TsWinRT.TsPosition source, TsWinRT.TsPosition target)
        {
            if (source.X == target.X)
            {
                if (source.Y == target.Y) return TsWinRT.TsBoardDirection.TsDirInvalid;
                return (source.Y < target.Y) ? 
                    TsWinRT.TsBoardDirection.TsDirDown : 
                    TsWinRT.TsBoardDirection.TsDirUp;
            }
            else if (source.Y == target.Y)
            {
                return (source.X < target.X) ?
                    TsWinRT.TsBoardDirection.TsDirRight :
                    TsWinRT.TsBoardDirection.TsDirLeft;
            }
            return TsWinRT.TsBoardDirection.TsDirInvalid;
        }
    }
}
