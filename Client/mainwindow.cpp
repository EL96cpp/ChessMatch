#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , board_scene(new QGraphicsScene)
    , player_taken_figures_scene(new QGraphicsScene)
    , opponent_taken_figures_scene(new QGraphicsScene)
    , pawn_transform_scene(new QGraphicsScene)
{

    ui->setupUi(this);
    setWindowTitle("Chess Match");
    SetFont();

    ui->boardGraphicsView->setScene(board_scene);
    ui->playerTakenView->setScene(opponent_taken_figures_scene);
    ui->opponentTakenView->setScene(player_taken_figures_scene);
    board = new Board(std::move(board_scene));
    taken_figures_manager = new TakenFiguresManager(std::move(opponent_taken_figures_scene),
                                                    std::move(player_taken_figures_scene));

    connect(board, &Board::SetMainWindowPlayerTurn, this, &MainWindow::SetPlayerTurn);
    connect(board, &Board::PlayerFigureTaken, taken_figures_manager, &TakenFiguresManager::AddPlayerTakenFigure);
    connect(board, &Board::OpponentFigureTaken, taken_figures_manager, &TakenFiguresManager::AddOpponentTakenFigure);
    connect(board, &Board::ShowTransformPawnChoice, this, &MainWindow::ShowTransformPawnChoice);
    connect(this, &MainWindow::SetPawnTransformChoice, board, &Board::SetPawnTransformChoice);
    connect(board, &Board::GameOver, this, &MainWindow::GameOver);

    ui->boardGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->boardGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->pawnTransformView->hide();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetPlayerTurn(const QString &turn)
{
    ui->game_info_label->setText(turn);
}

void MainWindow::ShowTransformPawnChoice(const QString& pawn_color)
{
    ui->pawnTransformView->setScene(pawn_transform_scene);
    ui->pawnTransformView->show();
    ui->pawnTransformView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    if (QString::compare(pawn_color, QString::fromLatin1("White"), Qt::CaseInsensitive) == 0) {

        SetWhitePawnTransformChoice();

    } else if (QString::compare(pawn_color, QString::fromLatin1("Black"), Qt::CaseInsensitive) == 0) {

        SetBlackPawnTransformChoice();

    }

}

void MainWindow::PawnTransformFigureClicked(ChessFigure* figure)
{
    for (auto& figure : pawn_transform_figures) {

        pawn_transform_scene->removeItem(figure);

    }
    pawn_transform_figures.clear();
    ui->pawnTransformView->hide();
    emit SetPawnTransformChoice(figure->GetType());
}

void MainWindow::GameOver(const QString &winner_color)
{
    QString winner = winner_color;
    winner += " wins!";
    ui->game_info_label->setText(winner);
}


void MainWindow::on_BoardStyleComboBox_currentTextChanged(const QString &arg1)
{
    if (QString::compare(arg1, QString::fromLatin1("Classic"), Qt::CaseInsensitive) == 0) {

        board->SetBrushes(QColor(255, 206, 158), QColor(209, 139, 71), QColor(54, 27, 0), QColor(0, 255, 224));

    } else if (QString::compare(arg1, QString::fromLatin1("Green"), Qt::CaseInsensitive) == 0) {

        board->SetBrushes(QColor(238, 238, 210), QColor(118, 150, 86), QColor(0, 61, 27), QColor(180, 145, 255));

    } else if (QString::compare(arg1, QString::fromLatin1("Blue"), Qt::CaseInsensitive) == 0) {

        board->SetBrushes(QColor(201, 216, 223), QColor(119, 154, 175), QColor(0, 75, 96), QColor(255, 140, 40));

    } else if (QString::compare(arg1, QString::fromLatin1("Red"), Qt::CaseInsensitive) == 0) {

        board->SetBrushes(QColor(193, 129, 130), QColor(110, 32, 32), QColor(37, 0, 27), QColor(104, 255, 84));

    }

}



void MainWindow::on_PlayerColorComboBox_currentTextChanged(const QString &player_color)
{

    ui->game_info_label->setText("White turn");

    if (QString::compare(player_color, QString::fromLatin1("White"), Qt::CaseInsensitive) == 0) {

        board->SetPlayerColor(FigureColor::WHITE);

    } else {

        board->SetPlayerColor(FigureColor::BLACK);

    }

    board->StartNewGame();
    taken_figures_manager->StartNewGame();

}


void MainWindow::on_NewGameButton_clicked()
{
    ui->game_info_label->setText("White turn");
    board->StartNewGame();
    taken_figures_manager->StartNewGame();
}

void MainWindow::SetFont()
{

    qint32 logo_font_id = QFontDatabase::addApplicationFont(":/Fonts/LogoFont.ttf");
    qint32 typing_font_id = QFontDatabase::addApplicationFont(":/Fonts/Font.ttf");
    QStringList logo_font_list = QFontDatabase::applicationFontFamilies(logo_font_id);
    QStringList typing_font_list = QFontDatabase::applicationFontFamilies(typing_font_id);
    QString logo_font_family = logo_font_list.first();
    QString typing_font_family = typing_font_list.first();

    ui->game_info_label->setFont(QFont(typing_font_family));
    ui->NewGameButton->setFont(QFont(typing_font_family));
    ui->board_style_label->setFont(QFont(typing_font_family));
    ui->player_color_label->setFont(QFont(typing_font_family));
    ui->BoardStyleComboBox->setFont(QFont(typing_font_family));
    ui->PlayerColorComboBox->setFont(QFont(typing_font_family));

}

void MainWindow::SetWhitePawnTransformChoice()
{
    int x = 0, y = 0;

    ChessFigure* rook = new Rook(y, x, FigureColor::WHITE, FigureOwner::PLAYER, this);
    connect(rook, &ChessFigure::FigureClicked, this, &MainWindow::PawnTransformFigureClicked);
    rook->setScale(0.6);
    pawn_transform_figures.push_back(rook);
    pawn_transform_scene->addItem(rook);

    ++y;
    ChessFigure* knight = new Knight(y, x, FigureColor::WHITE, FigureOwner::PLAYER, this);
    connect(knight, &ChessFigure::FigureClicked, this, &MainWindow::PawnTransformFigureClicked);
    knight->setScale(0.6);
    pawn_transform_figures.push_back(knight);
    pawn_transform_scene->addItem(knight);

    ++y;
    ChessFigure* bishop = new Bishop(y, x, FigureColor::WHITE, FigureOwner::PLAYER, this);
    connect(bishop, &ChessFigure::FigureClicked, this, &MainWindow::PawnTransformFigureClicked);
    bishop->setScale(0.6);
    pawn_transform_figures.push_back(bishop);
    pawn_transform_scene->addItem(bishop);

    ++y;
    ChessFigure* queen = new Queen(y, x, FigureColor::WHITE, FigureOwner::PLAYER, this);
    connect(queen, &ChessFigure::FigureClicked, this, &MainWindow::PawnTransformFigureClicked);
    queen->setScale(0.6);
    pawn_transform_figures.push_back(queen);
    pawn_transform_scene->addItem(queen);
}


void MainWindow::SetBlackPawnTransformChoice()
{
    int x = 0, y = 0;

    ChessFigure* rook = new Rook(y, x, FigureColor::BLACK, FigureOwner::PLAYER, this);
    connect(rook, &ChessFigure::FigureClicked, this, &MainWindow::PawnTransformFigureClicked);
    rook->setScale(0.6);
    pawn_transform_figures.push_back(rook);
    pawn_transform_scene->addItem(rook);

    ++y;
    ChessFigure* knight = new Knight(y, x, FigureColor::BLACK, FigureOwner::PLAYER, this);
    connect(knight, &ChessFigure::FigureClicked, this, &MainWindow::PawnTransformFigureClicked);
    knight->setScale(0.6);
    pawn_transform_figures.push_back(knight);
    pawn_transform_scene->addItem(knight);

    ++y;
    ChessFigure* bishop = new Bishop(y, x, FigureColor::BLACK, FigureOwner::PLAYER, this);
    connect(bishop, &ChessFigure::FigureClicked, this, &MainWindow::PawnTransformFigureClicked);
    bishop->setScale(0.6);
    pawn_transform_figures.push_back(bishop);
    pawn_transform_scene->addItem(bishop);

    ++y;
    ChessFigure* queen = new Queen(y, x, FigureColor::BLACK, FigureOwner::PLAYER, this);
    connect(queen, &ChessFigure::FigureClicked, this, &MainWindow::PawnTransformFigureClicked);
    queen->setScale(0.6);
    pawn_transform_figures.push_back(queen);
    pawn_transform_scene->addItem(queen);
}


