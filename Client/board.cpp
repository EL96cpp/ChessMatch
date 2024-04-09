#include "board.h"

Board::Board(QGraphicsScene* board_scene, QObject *parent)
    : QObject{parent}, board_scene(board_scene), player_color(FigureColor::WHITE),
      selected_figure(nullptr), transformation_pawn(nullptr), current_player(FigureColor::WHITE) {

    light_brush.setColor(QColor(255, 206, 158));
    light_brush.setStyle(Qt::SolidPattern);
    dark_brush.setColor(QColor(209, 139, 71));
    dark_brush.setStyle(Qt::SolidPattern);
    outline_brush.setColor(QColor(54, 27, 0));
    outline_brush.setStyle(Qt::SolidPattern);
    select_brush.setColor(QColor(0, 255, 224));
    select_brush.setStyle(Qt::SolidPattern);

    SetBoardOutlineCells();
    SetBoardOutlineText();
    SetBoardCells();
    PaintBoardCells();
    SetFigures();
    SetBoardNavigationMaps();

}

void Board::OnMakeMoveAccepted(const QString &letter_from, const QString &index_from, const QString &letter_to, const QString &index_to) {

    UnpaintSelectedFigureMoves();
    ChangeCurrentPlayer();

    if (player_color == FigureColor::WHITE) {

        MoveFigureToEmptyCell(white_board_navigation_map_y.key(index_from), white_board_navigation_map_x.key(letter_from),
                              white_board_navigation_map_y.key(index_to), white_board_navigation_map_x.key(letter_to));

    } else {

        MoveFigureToEmptyCell(black_board_navigation_map_y.key(index_from), black_board_navigation_map_x.key(letter_from),
                              black_board_navigation_map_y.key(index_to), black_board_navigation_map_x.key(letter_to));

    }

    selected_figure = nullptr;
    selected_figure_moves.clear();

}

void Board::OnEatFigureAccepted(const QString &letter_from, const QString &index_from, const QString &letter_to, const QString &index_to) {

    UnpaintSelectedFigureMoves();
    ChangeCurrentPlayer();

    if (player_color == FigureColor::WHITE) {

        size_t y_from = white_board_navigation_map_y.key(index_from);
        size_t x_from = white_board_navigation_map_x.key(letter_from);
        size_t y_to = white_board_navigation_map_y.key(index_to);
        size_t x_to = white_board_navigation_map_x.key(letter_to);

        FigureType figure_type = figures[y_to][x_to]->GetType();
        FigureColor figure_color = figures[y_to][x_to]->GetColor();


        if (figures[y_to][x_to]->GetColor() != player_color) {

            emit OpponentFigureTaken(figure_type, figure_color);

        } else {

            emit PlayerFigureTaken(figure_type, figure_color);

        }

        board_scene->removeItem(figures[y_to][x_to]);

        figures[y_to][x_to] = new EmptyFigure(y_to, x_to, this);
        figures[y_from][x_from]->SwapCoordinatesAndMovePixmaps(figures[y_to][x_to]);
        std::swap(figures[y_from][x_from], figures[y_to][x_to]);

    } else {

        size_t y_from = black_board_navigation_map_y.key(index_from);
        size_t x_from = black_board_navigation_map_x.key(letter_from);
        size_t y_to = black_board_navigation_map_y.key(index_to);
        size_t x_to = black_board_navigation_map_x.key(letter_to);

        FigureType figure_type = figures[y_to][x_to]->GetType();
        FigureColor figure_color = figures[y_to][x_to]->GetColor();

        if (figures[y_to][x_to]->GetColor() != player_color) {

            emit OpponentFigureTaken(figure_type, figure_color);

        } else {

            emit PlayerFigureTaken(figure_type, figure_color);

        }

        board_scene->removeItem(figures[y_to][x_to]);

        figures[y_to][x_to] = new EmptyFigure(y_to, x_to, this);
        figures[y_from][x_from]->SwapCoordinatesAndMovePixmaps(figures[y_to][x_to]);
        std::swap(figures[y_from][x_from], figures[y_to][x_to]);

    }

}

void Board::OnMakeCastlingAccepted(const QString &letter_from, const QString &index_from, const QString &letter_to, const QString &index_to) {

    UnpaintSelectedFigureMoves();
    ChangeCurrentPlayer();

    if (player_color == FigureColor::WHITE) {

        MakeFiguresCastling(white_board_navigation_map_y.key(index_from), white_board_navigation_map_x.key(letter_from),
                            white_board_navigation_map_y.key(index_to), white_board_navigation_map_x.key(letter_to));

    } else {

        MakeFiguresCastling(black_board_navigation_map_y.key(index_from), black_board_navigation_map_x.key(letter_from),
                            black_board_navigation_map_y.key(index_to), black_board_navigation_map_x.key(letter_to));

    }

    selected_figure = nullptr;
    selected_figure_moves.clear();

}

void Board::OnTransformPawnAccepted(const QString &letter_from, const QString &index_from, const QString &letter_to, const QString &index_to, const FigureType &figure_type) {



}

void Board::BoardCellClicked(BoardCell* cell) {

    if (waiting_for_pawn_transformation) {

        return;

    }

    if (selected_figure == nullptr) {

        // Set selected figure

        if (figures[cell->GetY()][cell->GetX()]->GetColor() == player_color &&
            figures[cell->GetY()][cell->GetX()]->GetColor() == current_player) {

            selected_figure = figures[cell->GetY()][cell->GetX()];
            selected_figure_moves = figures[cell->GetY()][cell->GetX()]->CalculateMoves(figures);
            PaintSelectedFigureMoves();

        }

    } else {

        if (selected_figure->GetX() == cell->GetX() && selected_figure->GetY() == cell->GetY()) {

            // Make selected figure nullptr

            UnpaintSelectedFigureMoves();
            selected_figure = nullptr;
            selected_figure_moves.clear();

        } else if (selected_figure->GetType() == FigureType::KING && !selected_figure->MadeFirstStep() &&
                   (cell->GetX() == selected_figure->GetX() + 2 || cell->GetX() == selected_figure->GetX() - 2)) {


            if (player_color == FigureColor::WHITE) {

                QString letter_from = white_board_navigation_map_x.value(selected_figure->GetX());
                QString index_from = white_board_navigation_map_y.value(selected_figure->GetY());
                QString letter_to = white_board_navigation_map_x.value(cell->GetX());
                QString index_to = white_board_navigation_map_y.value(cell->GetY());

                emit MakeCastling(letter_from, index_from, letter_to, index_to);

            } else if (player_color == FigureColor::BLACK) {

                QString letter_from = black_board_navigation_map_x.value(selected_figure->GetX());
                QString index_from = black_board_navigation_map_y.value(selected_figure->GetY());
                QString letter_to = black_board_navigation_map_x.value(cell->GetX());
                QString index_to = black_board_navigation_map_y.value(cell->GetY());

                emit MakeCastling(letter_from, index_from, letter_to, index_to);

            }


        } else if (selected_figure_moves.contains(std::make_pair(cell->GetY(), cell->GetX()))) {


            if (figures[cell->GetY()][cell->GetX()]->GetColor() == FigureColor::EMPTY) {

                if (player_color == FigureColor::WHITE) {

                    QString letter_from = white_board_navigation_map_x.value(selected_figure->GetX());
                    QString index_from = white_board_navigation_map_y.value(selected_figure->GetY());
                    QString letter_to = white_board_navigation_map_x.value(cell->GetX());
                    QString index_to = white_board_navigation_map_y.value(cell->GetY());

                    emit MakeMove(letter_from, index_from, letter_to, index_to);

                } else if (player_color == FigureColor::BLACK) {

                    QString letter_from = black_board_navigation_map_x.value(selected_figure->GetX());
                    QString index_from = black_board_navigation_map_y.value(selected_figure->GetY());
                    QString letter_to = black_board_navigation_map_x.value(cell->GetX());
                    QString index_to = black_board_navigation_map_y.value(cell->GetY());

                    emit MakeMove(letter_from, index_from, letter_to, index_to);

                }

            } else if (figures[cell->GetY()][cell->GetX()]->GetColor() != FigureColor::EMPTY) {

                // Eat figure

                if (player_color == FigureColor::WHITE) {

                    QString letter_from = white_board_navigation_map_x.value(selected_figure->GetX());
                    QString index_from = white_board_navigation_map_y.value(selected_figure->GetY());
                    QString letter_to = white_board_navigation_map_x.value(cell->GetX());
                    QString index_to = white_board_navigation_map_y.value(cell->GetY());

                    if (selected_figure->GetType() == FigureType::PAWN && cell->GetY() == 0) {

                        //emit EatFigure(letter_from, index_from, letter_to, index_to, );

                    } else {

                        emit EatFigure(letter_from, index_from, letter_to, index_to);

                    }



                } else if (player_color == FigureColor::BLACK) {

                    QString letter_from = black_board_navigation_map_x.value(selected_figure->GetX());
                    QString index_from = black_board_navigation_map_y.value(selected_figure->GetY());
                    QString letter_to = black_board_navigation_map_x.value(cell->GetX());
                    QString index_to = black_board_navigation_map_y.value(cell->GetY());

                    if (selected_figure->GetType() == FigureType::PAWN && cell->GetY() == 7) {



                    } else {

                        emit EatFigure(letter_from, index_from, letter_to, index_to);

                    }

                }

            }

            /*
            if (figures[cell->GetY()][cell->GetX()]->GetColor() == FigureColor::EMPTY) {





                // Move to empty cell

                MoveSelectedFigureToEmptyCell(cell->GetY(), cell->GetX());
                selected_figure = nullptr;
                selected_figure_moves.clear();

                // Also emits signal to change MainWidow data
                ChangeCurrentPlayer();



            } else {



               // Eat figure
                SelectedFigureTakesFigure(cell->GetY(), cell->GetX());
                selected_figure = nullptr;
                selected_figure_moves.clear();

                if (!game_over) {

                    ChangeCurrentPlayer();

                }



            }
            */

        } else {

            if (figures[cell->GetY()][cell->GetX()]->GetColor() == current_player &&
                figures[cell->GetY()][cell->GetX()]->GetColor() == player_color &&
                figures[cell->GetY()][cell->GetX()]->GetColor() == selected_figure->GetColor()) {

                // Set another selected_figure

                UnpaintSelectedFigureMoves();
                selected_figure_moves.clear();
                selected_figure = figures[cell->GetY()][cell->GetX()];
                selected_figure_moves = selected_figure->CalculateMoves(figures);
                PaintSelectedFigureMoves();

            }

        }

    }

}

void Board::FigureClicked(ChessFigure* figure) {

    if (waiting_for_pawn_transformation) {

        return;

    }

    if (selected_figure == nullptr) {

        if (figure->GetColor() == current_player && figure->GetColor() == player_color) {

            selected_figure = figure;
            selected_figure_moves = selected_figure->CalculateMoves(figures);
            PaintSelectedFigureMoves();

        }

    } else if (selected_figure->GetY() == figure->GetY() && selected_figure->GetX() == figure->GetX()) {

        UnpaintSelectedFigureMoves();
        selected_figure_moves.clear();
        selected_figure = nullptr;

    } else if (figure->GetColor() == current_player && figure->GetColor() == player_color) {

        UnpaintSelectedFigureMoves();
        selected_figure_moves.clear();
        selected_figure = figure;
        selected_figure_moves = selected_figure->CalculateMoves(figures);
        PaintSelectedFigureMoves();

    } else if (selected_figure_moves.contains(std::make_pair(figure->GetY(), figure->GetX()))) {

        //Need to send message

        if (player_color == FigureColor::WHITE) {

            QString letter_from = white_board_navigation_map_x.value(selected_figure->GetX());
            QString index_from = white_board_navigation_map_y.value(selected_figure->GetY());
            QString letter_to = white_board_navigation_map_x.value(figure->GetX());
            QString index_to = white_board_navigation_map_y.value(figure->GetY());

            emit MakeMove(letter_from, index_from, letter_to, index_to);

        } else if (player_color == FigureColor::BLACK) {

            QString letter_from = black_board_navigation_map_x.value(selected_figure->GetX());
            QString index_from = black_board_navigation_map_y.value(selected_figure->GetY());
            QString letter_to = black_board_navigation_map_x.value(figure->GetX());
            QString index_to = black_board_navigation_map_y.value(figure->GetY());

            emit MakeMove(letter_from, index_from, letter_to, index_to);

        }


        /*
        // Eat figure
        SelectedFigureTakesFigure(figure->GetY(), figure->GetX());
        selected_figure = nullptr;
        selected_figure_moves.clear();

        if (!game_over) {

            ChangeCurrentPlayer();

        }
        */

    }

}

void Board::StartNewGame() {

    board_scene->clear();
    board_cells.clear();
    board_outline_cells.clear();
    board_outline_text.clear();
    figures.clear();
    selected_figure = nullptr;
    selected_figure_moves.clear();
    game_over = false;
    transformation_pawn = nullptr;
    waiting_for_pawn_transformation = false;
    current_player = FigureColor::WHITE;

    SetBoardOutlineCells();
    SetBoardOutlineText();
    SetBoardCells();
    PaintBoardCells();
    SetFigures();

}

void Board::SetPlayerColor(const FigureColor &player_color) {

    this->player_color = player_color;

}

void Board::SetBrushes(const QColor& light_color, const QColor& dark_color, const QColor& outline_color, const QColor& select_color) {

    light_brush.setColor(light_color);
    light_brush.setStyle(Qt::SolidPattern);
    dark_brush.setColor(dark_color);
    dark_brush.setStyle(Qt::SolidPattern);
    outline_brush.setColor(outline_color);
    outline_brush.setStyle(Qt::SolidPattern);
    select_brush.setColor(select_color);
    select_brush.setStyle(Qt::SolidPattern);
    PaintBoardCells();

}

void Board::SetPawnTransformChoice(const FigureType &figure_type) {

    qDebug() << "Pawn transformation ";
    int y = transformation_pawn->GetY(), x = transformation_pawn->GetX();
    qDebug() << y << " " << x;
    board_scene->removeItem(figures[y][x]);
    FigureColor color = transformation_pawn->GetColor();
    FigureOwner owner = transformation_pawn->GetOwner();

    ChessFigure* figure;

    qDebug() << "switch next";

    switch(figure_type) {

    case FigureType::ROOK:
        qDebug() << "rook";
        figure = new Rook(y, x, color, owner, this);
        break;

    case FigureType::KNIGHT:
        qDebug() << "knight";
        figure = new Knight(y, x, color, owner, this);
        break;

    case FigureType::BISHOP:
        qDebug() << "bishop";
        figure = new Bishop(y, x, color, owner, this);
        break;

    case FigureType::QUEEN:
        qDebug() << "queen";
        figure = new Queen(y, x, color, owner, this);
        break;

    }

    qDebug() << "switch ends";
    connect(figure, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
    figure->SetMadeFirstStep(true);
    board_scene->addItem(figure);
    figures[y][x] = std::move(figure);

    transformation_pawn = nullptr;
    waiting_for_pawn_transformation = false;

}

void Board::SetBoardNavigationMaps() {

    white_board_navigation_map_x[0] = "a";
    white_board_navigation_map_x[1] = "b";
    white_board_navigation_map_x[2] = "c";
    white_board_navigation_map_x[3] = "d";
    white_board_navigation_map_x[4] = "e";
    white_board_navigation_map_x[5] = "f";
    white_board_navigation_map_x[6] = "g";
    white_board_navigation_map_x[7] = "h";

    white_board_navigation_map_y[0] = "8";
    white_board_navigation_map_y[1] = "7";
    white_board_navigation_map_y[2] = "6";
    white_board_navigation_map_y[3] = "5";
    white_board_navigation_map_y[4] = "4";
    white_board_navigation_map_y[5] = "3";
    white_board_navigation_map_y[6] = "2";
    white_board_navigation_map_y[7] = "1";


    black_board_navigation_map_x[0] = "h";
    black_board_navigation_map_x[1] = "g";
    black_board_navigation_map_x[2] = "f";
    black_board_navigation_map_x[3] = "e";
    black_board_navigation_map_x[4] = "d";
    black_board_navigation_map_x[5] = "c";
    black_board_navigation_map_x[6] = "b";
    black_board_navigation_map_x[7] = "a";

    black_board_navigation_map_y[0] = "1";
    black_board_navigation_map_y[1] = "2";
    black_board_navigation_map_y[2] = "3";
    black_board_navigation_map_y[3] = "4";
    black_board_navigation_map_y[4] = "5";
    black_board_navigation_map_y[5] = "6";
    black_board_navigation_map_y[6] = "7";
    black_board_navigation_map_y[7] = "8";

}

void Board::MoveFigureToEmptyCell(const int& y_from, const int& x_from, const int& y_to, const int& x_to) {

    figures[y_from][x_from]->SwapCoordinatesAndMovePixmaps(figures[y_to][x_to]);

    std::swap(figures[y_from][x_from], figures[y_to][x_to]);

}

void Board::MakeFiguresCastling(const int &y_from, const int &x_from, const int &y_to, const int &x_to) {

    if (x_from > x_to) {

        //Left castling

        figures[y_from][x_from]->SwapCoordinatesAndMovePixmaps(figures[y_to][x_to]);
        std::swap(figures[y_from][x_from], figures[y_to][x_to]);

        figures[y_from][0]->SwapCoordinatesAndMovePixmaps(figures[y_to][x_to+1]);
        std::swap(figures[y_from][0], figures[y_to][x_to+1]);

    } else {

        //Right castling

        figures[y_from][x_from]->SwapCoordinatesAndMovePixmaps(figures[y_to][x_to]);
        std::swap(figures[y_from][x_from], figures[y_to][x_to]);

        figures[y_from][7]->SwapCoordinatesAndMovePixmaps(figures[y_to][x_to-1]);
        std::swap(figures[y_from][7], figures[y_to][x_to-1]);

    }

}

void Board::SelectedFigureTakesFigure(const int &taken_y, const int &taken_x) {

    UnpaintSelectedFigureMoves();
    QPointF taken_pos = figures[taken_y][taken_x]->pos();
    int selected_y = selected_figure->GetY(), selected_x = selected_figure->GetX();
    board_scene->removeItem(figures[taken_y][taken_x]);

    if (figures[taken_y][taken_x]->GetColor() != player_color) {

        if (figures[taken_y][taken_x]->GetType() == FigureType::KING) {

            QString winner = (selected_figure->GetColor() == FigureColor::WHITE) ? ("White") : ("Black");
            game_over = true;
            emit GameOver(winner);

        }

        //emit OpponentFigureTaken(std::move(figures[taken_y][taken_x]));

    } else {

        if (figures[taken_y][taken_x]->GetType() == FigureType::KING) {

            QString winner = (selected_figure->GetColor() == FigureColor::WHITE) ? ("White") : ("Black");
            game_over = true;
            emit GameOver(winner);

        }

        //emit PlayerFigureTaken(std::move(figures[taken_y][taken_x]));

    }

    selected_figure->SetY(taken_y);
    selected_figure->SetX(taken_x);
    selected_figure->setPos(taken_pos);
    selected_figure->SetMadeFirstStep(true);
    figures[taken_y][taken_x] = std::move(selected_figure);
    figures[selected_y][selected_x] = new EmptyFigure(selected_y, selected_x, this);

    if (figures[taken_y][taken_x]->GetType() == FigureType::PAWN) {

        if (figures[taken_y][taken_x]->GetOwner() == FigureOwner::PLAYER && taken_y == 0) {

            transformation_pawn = figures[taken_y][taken_x];
            QString pawn_color = (figures[taken_y][taken_x]->GetColor() == FigureColor::WHITE) ? ("White") : ("Black");
            emit ShowTransformPawnChoice(pawn_color);
            waiting_for_pawn_transformation = true;

        } else if (figures[taken_y][taken_x]->GetOwner() == FigureOwner::OPPONENT && taken_y == 7) {

            transformation_pawn = figures[taken_y][taken_x];
            QString pawn_color = (figures[taken_y][taken_x]->GetColor() == FigureColor::WHITE) ? ("White") : ("Black");
            emit ShowTransformPawnChoice(pawn_color);
            waiting_for_pawn_transformation = true;

        }

    }

}

void Board::ChangeCurrentPlayer() {

    if (current_player == FigureColor::WHITE) {

        current_player = FigureColor::BLACK;
        emit SetMainWindowPlayerTurn(QString::fromLatin1("Black turn"));

    } else {

        current_player = FigureColor::WHITE;
        emit SetMainWindowPlayerTurn(QString::fromLatin1("White turn"));

    }

}

void Board::SetBoardOutlineText() {

    QVector<QGraphicsTextItem*> bottom_text;
    QVector<QGraphicsTextItem*> left_text;
    QVector<QString> letters = {"A", "B", "C", "D", "E", "F", "G", "H"};
    QVector<QString> numbers = {"1", "2", "3", "4", "5", "6", "7", "8"};
    int x_start = 80, y_start = 80;

    if (player_color == FigureColor::WHITE) {

        for (int i = 0; i < 8; ++i) {

            bottom_text.push_back(new QGraphicsTextItem(letters[i]));
            bottom_text[i]->setPos(x_start, 865);
            bottom_text[i]->setDefaultTextColor(QColor(255, 255, 255));
            board_scene->addItem(bottom_text[i]);
            x_start += 100;

            left_text.push_back(new QGraphicsTextItem(numbers[7-i]));
            left_text[i]->setPos(20, y_start);
            left_text[i]->setDefaultTextColor(QColor(255, 255, 255));
            board_scene->addItem(left_text[i]);
            y_start += 100;

        }

    } else {

        for (int i = 0; i < 8; ++i) {

            bottom_text.push_back(new QGraphicsTextItem(letters[7-i]));
            bottom_text[i]->setPos(x_start, 865);
            bottom_text[i]->setDefaultTextColor(QColor(255, 255, 255));
            board_scene->addItem(bottom_text[i]);
            x_start += 100;

            left_text.push_back(new QGraphicsTextItem(numbers[i]));
            left_text[i]->setPos(20, y_start);
            left_text[i]->setDefaultTextColor(QColor(255, 255, 255));
            board_scene->addItem(left_text[i]);
            y_start += 100;

        }

    }
    board_outline_text.push_back(bottom_text);
    board_outline_text.push_back(left_text);
}

void Board::SetBoardCells() {

    for (int i = 0; i < 8; ++i) {

        QVector<BoardCell*> board_line;

        for (int j = 0; j < 8; ++j) {

            board_line.push_back(new BoardCell(i, j, 100, 100));
            connect(board_line[j], &BoardCell::BoardCellClicked, this, &Board::BoardCellClicked);
            board_scene->addItem(board_line[j]);

        }

        board_cells.push_back(board_line);

    }

}

void Board::PaintBoardCells() {

    for (auto& line : board_outline_cells) {

        for (auto& cell : line) {

            cell->setBrush(outline_brush);

        }

    }

    for (int i = 0; i < 8; ++i) {

        for (int j = 0; j < 8; ++j) {

            if ((i + j) % 2 == 0) {

                board_cells[i][j]->setBrush(light_brush);

            } else {

                board_cells[i][j]->setBrush(dark_brush);

            }

        }

    }

}

void Board::SetFigures() {

    // Order of function calls is critical!
    SetOpponentFigures();
    SetEmptyFigures();
    SetPlayerFigures();

}

void Board::SetOpponentFigures() {

    QVector<ChessFigure*> line;
    QVector<ChessFigure*> pawn_line;

    if (player_color == FigureColor::WHITE) {

        ChessFigure* rook_left = new Rook(0, 0, FigureColor::BLACK, FigureOwner::OPPONENT, this);
        connect(rook_left, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(rook_left));
        board_scene->addItem(*line.rbegin());

        ChessFigure* knight_left = new Knight(0, 1, FigureColor::BLACK, FigureOwner::OPPONENT, this);
        connect(knight_left, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(knight_left));
        board_scene->addItem(*line.rbegin());

        ChessFigure* bishop_left = new Bishop(0, 2, FigureColor::BLACK, FigureOwner::OPPONENT, this);
        connect(bishop_left, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(bishop_left));
        board_scene->addItem(*line.rbegin());

        ChessFigure* queen = new Queen(0, 3, FigureColor::BLACK, FigureOwner::OPPONENT, this);
        connect(queen, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(queen));
        board_scene->addItem(*line.rbegin());

        ChessFigure* king = new King(0, 4, FigureColor::BLACK, FigureOwner::OPPONENT, this);
        connect(king, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(king));
        board_scene->addItem(*line.rbegin());

        ChessFigure* bishop_right = new Bishop(0, 5, FigureColor::BLACK, FigureOwner::OPPONENT, this);
        connect(bishop_right, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(bishop_right));
        board_scene->addItem(*line.rbegin());

        ChessFigure* knight_right = new Knight(0, 6, FigureColor::BLACK, FigureOwner::OPPONENT, this);
        connect(knight_right, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(knight_right));
        board_scene->addItem(*line.rbegin());

        ChessFigure* rook_right = new Rook(0, 7, FigureColor::BLACK, FigureOwner::OPPONENT, this);
        connect(rook_right, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(rook_right));
        board_scene->addItem(*line.rbegin());

        figures.push_back(line);

        for (int i = 0; i < 8; ++i) {

            ChessFigure* pawn = new Pawn(1, i, FigureColor::BLACK, FigureOwner::OPPONENT, this);
            connect(pawn, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
            pawn_line.push_back(std::move(pawn));
            board_scene->addItem(*pawn_line.rbegin());

        }

        figures.push_back(pawn_line);

    } else {

        ChessFigure* rook_left = new Rook(0, 0, FigureColor::WHITE, FigureOwner::OPPONENT, this);
        connect(rook_left, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(rook_left));
        board_scene->addItem(*line.rbegin());

        ChessFigure* knight_left = new Knight(0, 1, FigureColor::WHITE, FigureOwner::OPPONENT, this);
        connect(knight_left, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(knight_left));
        board_scene->addItem(*line.rbegin());

        ChessFigure* bishop_left = new Bishop(0, 2, FigureColor::WHITE, FigureOwner::OPPONENT, this);
        connect(bishop_left, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(bishop_left));
        board_scene->addItem(*line.rbegin());

        ChessFigure* king = new King(0, 3, FigureColor::WHITE, FigureOwner::OPPONENT, this);
        connect(king, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(king));
        board_scene->addItem(*line.rbegin());

        ChessFigure* queen = new Queen(0, 4, FigureColor::WHITE, FigureOwner::OPPONENT, this);
        connect(queen, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(queen));
        board_scene->addItem(*line.rbegin());

        ChessFigure* bishop_right = new Bishop(0, 5, FigureColor::WHITE, FigureOwner::OPPONENT, this);
        connect(bishop_right, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(bishop_right));
        board_scene->addItem(*line.rbegin());

        ChessFigure* knight_right = new Knight(0, 6, FigureColor::WHITE, FigureOwner::OPPONENT, this);
        connect(knight_right, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(knight_right));
        board_scene->addItem(*line.rbegin());

        ChessFigure* rook_right = new Rook(0, 7, FigureColor::WHITE, FigureOwner::OPPONENT, this);
        connect(rook_right, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(rook_right));
        board_scene->addItem(*line.rbegin());

        figures.push_back(line);

        for (int i = 0; i < 8; ++i) {

            ChessFigure* pawn = new Pawn(1, i, FigureColor::WHITE, FigureOwner::OPPONENT, this);
            connect(pawn, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
            pawn_line.push_back(std::move(pawn));
            board_scene->addItem(*pawn_line.rbegin());

        }

        figures.push_back(pawn_line);

    }

}

void Board::SetEmptyFigures() {

    for (int i = 2; i <= 5; ++i) {

        QVector<ChessFigure*> empty_line;

        for (int j = 0; j < 8; ++j) {

            ChessFigure* figure = new EmptyFigure(i, j, this);
            empty_line.push_back(std::move(figure));

        }

        figures.push_back(empty_line);

    }

}

void Board::SetPlayerFigures() {

    QVector<ChessFigure*> line;
    QVector<ChessFigure*> pawn_line;

    if (player_color == FigureColor::WHITE) {

        for (int i = 0; i < 8; ++i) {

            ChessFigure* pawn = new Pawn(6, i, FigureColor::WHITE, FigureOwner::PLAYER, this);
            connect(pawn, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
            pawn_line.push_back(std::move(pawn));
            board_scene->addItem(*pawn_line.rbegin());

        }

        ChessFigure* rook_left = new Rook(7, 0, FigureColor::WHITE, FigureOwner::PLAYER, this);
        connect(rook_left, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(rook_left));
        board_scene->addItem(*line.rbegin());

        ChessFigure* knight_left = new Knight(7, 1, FigureColor::WHITE, FigureOwner::PLAYER, this);
        connect(knight_left, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(knight_left));
        board_scene->addItem(*line.rbegin());

        ChessFigure* bishop_left = new Bishop(7, 2, FigureColor::WHITE, FigureOwner::PLAYER, this);
        connect(bishop_left, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(bishop_left));
        board_scene->addItem(*line.rbegin());

        ChessFigure* queen = new Queen(7, 3, FigureColor::WHITE, FigureOwner::PLAYER, this);
        connect(queen, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(queen));
        board_scene->addItem(*line.rbegin());

        ChessFigure* king = new King(7, 4, FigureColor::WHITE, FigureOwner::PLAYER, this);
        connect(king, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(king));
        board_scene->addItem(*line.rbegin());

        ChessFigure* bishop_right = new Bishop(7, 5, FigureColor::WHITE, FigureOwner::PLAYER, this);
        connect(bishop_right, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(bishop_right));
        board_scene->addItem(*line.rbegin());

        ChessFigure* knight_right = new Knight(7, 6, FigureColor::WHITE, FigureOwner::PLAYER, this);
        connect(knight_right, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(knight_right));
        board_scene->addItem(*line.rbegin());

        ChessFigure* rook_right = new Rook(7, 7, FigureColor::WHITE, FigureOwner::PLAYER, this);
        connect(rook_right, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(rook_right));
        board_scene->addItem(*line.rbegin());

    } else {

        for (int i = 0; i < 8; ++i) {

            ChessFigure* pawn = new Pawn(6, i, FigureColor::BLACK, FigureOwner::PLAYER, this);
            connect(pawn, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
            pawn_line.push_back(std::move(pawn));
            board_scene->addItem(*pawn_line.rbegin());

        }

        ChessFigure* rook_left = new Rook(7, 0, FigureColor::BLACK, FigureOwner::PLAYER, this);
        connect(rook_left, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(rook_left));
        board_scene->addItem(*line.rbegin());

        ChessFigure* knight_left = new Knight(7, 1, FigureColor::BLACK, FigureOwner::PLAYER, this);
        connect(knight_left, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(knight_left));
        board_scene->addItem(*line.rbegin());

        ChessFigure* bishop_left = new Bishop(7, 2, FigureColor::BLACK, FigureOwner::PLAYER, this);
        connect(bishop_left, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(bishop_left));
        board_scene->addItem(*line.rbegin());

        ChessFigure* king = new King(7, 3, FigureColor::BLACK, FigureOwner::PLAYER, this);
        connect(king, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(king));
        board_scene->addItem(*line.rbegin());

        ChessFigure* queen = new Queen(7, 4, FigureColor::BLACK, FigureOwner::PLAYER, this);
        connect(queen, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(queen));
        board_scene->addItem(*line.rbegin());

        ChessFigure* bishop_right = new Bishop(7, 5, FigureColor::BLACK, FigureOwner::PLAYER, this);
        connect(bishop_right, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(bishop_right));
        board_scene->addItem(*line.rbegin());

        ChessFigure* knight_right = new Knight(7, 6, FigureColor::BLACK, FigureOwner::PLAYER, this);
        connect(knight_right, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(knight_right));
        board_scene->addItem(*line.rbegin());

        ChessFigure* rook_right = new Rook(7, 7, FigureColor::BLACK, FigureOwner::PLAYER, this);
        connect(rook_right, &ChessFigure::FigureClicked, this, &Board::FigureClicked);
        line.push_back(std::move(rook_right));
        board_scene->addItem(*line.rbegin());

    }

    figures.push_back(pawn_line);
    figures.push_back(line);

}

void Board::PaintSelectedFigureMoves() {

    board_cells[selected_figure->GetY()][selected_figure->GetX()]->setBrush(select_brush);

    for (auto& pair : selected_figure_moves) {

        board_cells[pair.first][pair.second]->setBrush(select_brush);

    }

}

void Board::UnpaintSelectedFigureMoves() {

    if (selected_figure == nullptr) {

        return;

    }

    if ((selected_figure->GetY() + selected_figure->GetX()) % 2 == 0) {

        board_cells[selected_figure->GetY()][selected_figure->GetX()]->setBrush(light_brush);

    } else {

        board_cells[selected_figure->GetY()][selected_figure->GetX()]->setBrush(dark_brush);

    }

    for (auto& pair : selected_figure_moves) {

        if ((pair.first + pair.second) % 2 == 0) {

            board_cells[pair.first][pair.second]->setBrush(light_brush);

        } else {

            board_cells[pair.first][pair.second]->setBrush(dark_brush);

        }

    }

}

void Board::SetBoardOutlineCells() {

    QVector<QGraphicsRectItem*> corner_cells;
    corner_cells.push_back(new QGraphicsRectItem(0, 0, 50, 50));
    corner_cells.push_back(new QGraphicsRectItem(850, 0, 50, 50));
    corner_cells.push_back(new QGraphicsRectItem(850, 850, 50, 50));
    corner_cells.push_back(new QGraphicsRectItem(0, 850, 50, 50));

    for (int i = 0; i < corner_cells.size(); ++i) {

        board_scene->addItem(corner_cells[i]);

    }

    board_outline_cells.push_back(corner_cells);

    QVector<QGraphicsRectItem*> top_cells;
    QVector<QGraphicsRectItem*> bottom_cells;
    int x_start = 50;

    for (int i = 0; i < 8; ++i) {

        top_cells.push_back(new QGraphicsRectItem(x_start, 0, 100, 50));
        bottom_cells.push_back(new QGraphicsRectItem(x_start, 850, 100, 50));
        board_scene->addItem(top_cells[i]);
        board_scene->addItem(bottom_cells[i]);
        x_start += 100;

    }

    board_outline_cells.push_back(top_cells);
    board_outline_cells.push_back(bottom_cells);

    QVector<QGraphicsRectItem*> right_cells;
    QVector<QGraphicsRectItem*> left_cells;
    int y_start = 50;

    for (int i = 0; i < 8; ++i) {

        right_cells.push_back(new QGraphicsRectItem(850, y_start, 50, 100));
        left_cells.push_back(new QGraphicsRectItem(0, y_start, 50, 100));
        board_scene->addItem(right_cells[i]);
        board_scene->addItem(left_cells[i]);
        y_start += 100;

    }

    board_outline_cells.push_back(right_cells);
    board_outline_cells.push_back(left_cells);

}
