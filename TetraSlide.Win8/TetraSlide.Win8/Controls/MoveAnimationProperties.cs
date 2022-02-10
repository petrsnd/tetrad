using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Media.Animation;

namespace TetraSlide.Win8.Controls
{
    class MoveAnimationProperties
    {
        public MoveAnimationProperties(Board board, Tile t, TsWinRT.TsPosition tilePos, TsWinRT.TsBoardDirection dir, int distance)
        {
            TsWinRT.TsPosition newPos = tilePos;
            switch (dir)
            {
                case TsWinRT.TsBoardDirection.TsDirDown:
                    newPos.Y += distance;
                    AnimationProperty = "Y";
                    break;
                case TsWinRT.TsBoardDirection.TsDirUp:
                    newPos.Y -= distance;
                    AnimationProperty = "Y";
                    break;
                case TsWinRT.TsBoardDirection.TsDirLeft:
                    newPos.X -= distance;
                    AnimationProperty = "X";
                    break;
                case TsWinRT.TsBoardDirection.TsDirRight:
                    newPos.X += distance;
                    AnimationProperty = "X";
                    break;
            }
            var bs = board.GetBoardSpace(newPos);
            var tileTopLeft = t.TransformToVisual(board).TransformPoint(new Point(0, 0));
            var targetTopLeft = bs.TransformToVisual(board).TransformPoint(new Point(0, 0));
            double AnimationValue = targetTopLeft.X - tileTopLeft.X;
            if (AnimationProperty == "Y")
                AnimationValue = targetTopLeft.Y - tileTopLeft.Y;
        }

        public string AnimationProperty
        {
            get;
            private set;
        }

        double AnimationValue
        {
            get;
            set;
        }

        public Timeline CreateAnimation()
        {
            DoubleAnimation da = new DoubleAnimation();
            da.To = AnimationValue;
            da.Duration = new Duration(new TimeSpan(0, 0, 0, 0, 500));
            return da;
        }
    };
}
