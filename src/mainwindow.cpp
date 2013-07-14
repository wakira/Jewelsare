#include <QGridLayout>
#include <QPixmap>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLabel>
#include <QFrame>
#include "util/jewelbutton.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamestate.h"

using namespace Jewelsare;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	game_state_ = new GameState();

	current_frame_ = 0;
	StartHome_();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::StartHome_()
{
	if(current_frame_)
		delete current_frame_;

	current_frame_ = new QFrame(ui->centralWidget);
	QGridLayout* layout = new QGridLayout(current_frame_);

	JewelButton *start_button,*score_button;
	start_button = new JewelButton(current_frame_);
	score_button = new JewelButton(current_frame_);
	QLabel *banner = new QLabel(current_frame_);
	banner->setText(tr("Jewelsare"));
	start_button->setText(tr("START GAME"));
	score_button->setText(tr("SCORE BOARD"));
	layout->addWidget(start_button,1,0);
	layout->addWidget(score_button,2,0);
	layout->addWidget(banner,0,0);
	current_frame_->show();
	QObject::connect(start_button,SIGNAL(clicked()),this,SLOT(StartClicked()));
	QObject::connect(score_button,SIGNAL(clicked()),this,SLOT(ScoreClicked()));
}

void MainWindow::StartClicked()
{
	StartSelect_();
}

void MainWindow::ScoreClicked()
{

}

void MainWindow::StartSelect_()
{
	if(current_frame_)
		delete current_frame_;

	current_frame_ = new QFrame(ui->centralWidget);
	QGridLayout* layout = new QGridLayout(current_frame_);

	JewelButton *go_button = new JewelButton(current_frame_);
	go_button->setText(tr("GO"));
	layout->addWidget(go_button,2,0);
	connect(go_button,SIGNAL(clicked()),this,SLOT(GoClicked()));

	QButtonGroup *mode_group = new QButtonGroup(current_frame_);
	QButtonGroup *difficulty_group = new QButtonGroup(current_frame_);
	//TODO add mode and difficulty select, also a "HOME" button
	QRadioButton *time_radio = new QRadioButton("Time Limited Mode",current_frame_);
	QRadioButton *reaction_radio = new QRadioButton("Reaction Mode",current_frame_);
	QRadioButton *easy = new QRadioButton("EASY",current_frame_);
	QRadioButton *medium = new QRadioButton("MEDIUM",current_frame_);
	QRadioButton *hard = new QRadioButton("HARD",current_frame_);
	mode_group->addButton(time_radio,0);
	mode_group->addButton(reaction_radio,1);
	difficulty_group->addButton(easy,0);
	difficulty_group->addButton(medium,1);
	difficulty_group->addButton(hard,2);
	layout->addWidget(time_radio,0,0);
	layout->addWidget(reaction_radio,0,1);
	layout->addWidget(easy,1,0);
	layout->addWidget(medium,1,1);
	layout->addWidget(hard,1,2);

	//default
	easy->setChecked(true);
	time_radio->setChecked(true);

	mode_group_ = mode_group;
	difficulty_group_ = difficulty_group;
	current_frame_->show();
}

void MainWindow::GoClicked()
{
	if(mode_group_->checkedId() == 1)
		game_state_->SetMode(Mode::FAST_REACTION);
	else
		game_state_->SetMode(Mode::TIME_LIMIT);

	switch(difficulty_group_->checkedId()) {
	case 0:
		game_state_->SetDifficulty(Difficulty::EASY);
		break;
	case 1:
		game_state_->SetDifficulty(Difficulty::MEDIUM);
		break;
	case 2:
		game_state_->SetDifficulty(Difficulty::HARD);
		break;
	default:
		game_state_->SetDifficulty(Difficulty::EASY);
		break;
	}

	StartGame_();
}

void MainWindow::OnSwap(JewelPos pos,JewelWidget::SwapDirection direction)
{
	auto events = game_state_->Swap(pos,direction);
	bool swaped = false;
	for(BoardEvent event : events) {
		swaped = true;
		//TODO update ui according to event
		switch(event.type) {
		case BoardEvent::EventType::DIE:
		case BoardEvent::EventType::NEW:
		case BoardEvent::EventType::FALL:
			break;
		}
	}
	if(!swaped) {
		//TODO swap fail animation
	}
}

void MainWindow::StartGame_()
{
	//TODO Set new UI frame
	delete current_frame_;
	current_frame_ = new QFrame(ui->centralWidget);

	QLabel *board = new QLabel(current_frame_);
	QGridLayout *jewel_layout = new QGridLayout(board);
	board->setPixmap(QPixmap("board.png"));
	current_frame_->show();
	game_state_->StartNewGame();
}
