using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.ComponentModel;
using System.Net;
using System.Net.Sockets;

namespace Devices
{
    class ShipPlacementChecker
    {
        private Button[,] board;

        public Button[,] Board { get { return board; } }

        public ShipPlacementChecker(Button[,] buttonArray)
        {
            board = buttonArray;
        }

        public bool IsHorizontalSelectionOutOfRange(int i, int j, int length)
        {
            if(i + length > board.GetLength(0))
            {
                return true;
            }
            return false;
        }

        public bool IsVerticalSelectionOutOfRange(int i, int j, int length)
        {
            if(j + length > board.GetLength(0))
            {
                return true;
            }
            return false;
        }

        public bool IsCoordinateEmpty(int i, int j)
        {
            if (board[i, j].IsEnabled)
            {
                return true;
            }
            return false;
        }

        public bool IsSelectedHorizontalPlaceEmpty(int i, int j, int length)
        {
            if (!IsHorizontalSelectionOutOfRange(i, j, length))
            {
                for (int step = 0; step < length; step++)
                {
                    if (!IsCoordinateEmpty(i + step, j))
                    {
                        return false;
                    }
                }
                return true;
            }
            return false;
        }

        public bool IsSelectedVerticalPlaceEmpty(int i, int j, int length)
        {
            if (!IsVerticalSelectionOutOfRange(i, j, length))
            {
                for (int step = 0; step < length; step++)
                {
                    if (!IsCoordinateEmpty(i, j + step))
                    {
                        return false;
                    }
                }
                return true;
            }
            return false;
        }

        public void VerticalPlaceTheShip(int i, int j, int length)
        {
            if(IsSelectedVerticalPlaceEmpty(i, j, length))
            {
                for (int step = 0; step < length; step++)
                {
                    board[i, j + step].IsEnabled = false;
                }
            }
        }

        public void HorizontalPlaceTheShip(int i, int j, int length)
        {
            if (IsSelectedHorizontalPlaceEmpty(i, j, length))
            {
                for (int step = 0; step < length; step++)
                {
                    board[i + step, j].IsEnabled = false;
                }
            }
        }
    }
}
