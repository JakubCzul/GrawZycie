#include "gameboard.h"
#include <QTimer>
#include <QVector>
#include <QDebug>

GameOfLifeBoard::GameOfLifeBoard(QWidget *parent)
    : QTableWidget(parent), timer(new QTimer(this))
{
    connect(this, &QTableWidget::cellClicked, this, &GameOfLifeBoard::cellClicked);
    connect(timer, &QTimer::timeout, this, &GameOfLifeBoard::play);
    initializeBoard();
    timer->start(1000);
}

void GameOfLifeBoard::initializeBoard() {
    setRowCount(10);
    setColumnCount(10);

    for (int row = 0; row < rowCount(); row++) {
        for (int col = 0; col < columnCount(); col++) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            setItem(row, col, item);
        }
    }
}

void GameOfLifeBoard::cellClicked(int row, int column) {
    QTableWidgetItem *item = this->item(row,column);
    item->setBackground(item->background() == Qt::white ? Qt::black : Qt::white);
}

void GameOfLifeBoard::play() {
    updateBoard();
}

void GameOfLifeBoard::faster() {
    int interval = timer->interval();
    if (interval > 50) {
        timer->setInterval(interval - 50);
    }
}

void GameOfLifeBoard::stop() {
    timer->stop();
}


void GameOfLifeBoard::updateBoard() {
    int rows = rowCount();
    int cols = columnCount();
    QVector<QVector<int>> newBoard(rows, QVector<int>(cols, 0));

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int liveNeighbors = countLiveNeighbors(row, col);

            // Zasady gry w życie
            if (item(row, col)->background() == Qt::black) {
                if (liveNeighbors < 2 || liveNeighbors > 3) {
                    newBoard[row][col] = 0;  // Zasada 1 i 3
                } else {
                    newBoard[row][col] = 1;  // Zasada 2
                }
            } else {
                // Komórka jest nieaktywna (biała)
                if (liveNeighbors == 3) {
                    newBoard[row][col] = 1;  // Zasada 4
                }
            }
        }
    }

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            QTableWidgetItem *item = this->item(row, col);
            item->setBackground(newBoard[row][col] == 1 ? Qt::black : Qt::white);
        }
    }
}

int GameOfLifeBoard::countLiveNeighbors(int row, int column) {
    int count = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int newRow = row + i;
            int newCol = column + j;

            if (newRow >= 0 && newRow < rowCount() && newCol >= 0 && newCol < columnCount() &&
                !(i == 0 && j == 0)) {
                count += item(newRow, newCol)->background() == Qt::black ? 1 : 0;
            }
        }
    }
    return count;
}