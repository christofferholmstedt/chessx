/***************************************************************************
								  BoardView - view of the current board
									  -------------------
	 begin                : Sun 21 Aug 2005
	 copyright            : (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef BOARDVIEW_H_INCLUDED
#define BOARDVIEW_H_INCLUDED

#include "board.h"
#include "boardtheme.h"
#include "guess.h"
#include "threadedguess.h"

#include <QWidget>
#include <QPointer>

using namespace chessx;

class BoardTheme;

/** @ingroup GUI
The BoardView class represents a widget for displaying current
position on the screen. */

class BoardView : public QWidget
{
    Q_OBJECT
public:
    enum {WheelUp = Qt::LeftButton, WheelDown = Qt::RightButton};
    enum {Automatic = 0, Always = 1, Never = 2};
    enum {IgnoreSideToMove = 1, SuppressGuessMove = 2, AllowCopyPiece = 4, AllowCustomBackground = 8};
    typedef enum {ActionStandard, ActionQuery, ActionReplace, ActionInsert, ActionAdd, ActionPen, ActionAskEngine, ActionEvalMove } BoardViewAction;
    /** Create board widget. */
    BoardView(QWidget* parent = nullptr, int flags = 0);
    /** Destroy widget. */
    ~BoardView();
    /** Set flags for board. Flags include:
    * @p IgnoreSideToMove - allow dragging all pieces (useful for setting up a position)
    */
    BoardView* boardView() { return this; }
    void setFlags(int flags);
    int flags() const;
    /** Update and shows current position. */
    void setBoard(const BoardX& value, Square from = InvalidSquare, Square to = InvalidSquare, bool atLineEnd = true);
    /** @return displayed position. */
    BoardX board() const;
    /** @return current theme */
    const BoardTheme& theme() const;
    /** @return @p true if board is displayed upside down. */
    bool isFlipped() const;
    /** Make it almost square. */
    virtual int heightForWidth(int width) const;
    /** Switch to next guess */
    void nextGuess(Square s);
    /** Set Move Indicator property */
    void showMoveIndicator(bool visible);
    bool showMoveIndicator() const;
    /** Set Move Indicator property */
    void showCoordinates(bool visible);
    bool showCoordinates() const;
    /** Set a reference to the database to which the current view is associated */
    void setDbIndex(QObject *);
    /** Get a reference to the database to which the current view is associated */
    QObject *dbIndex() const;
    /** Get the move indicator width */
    int moveIndicatorWidth(int width, int themeWidth) const;

    bool vAlignTop() const;
    void setVAlignTop(bool vAlignTop);

    Piece dragged() const;
    void setDragged(const Piece &dragged);

    void renderImage(QImage& image, double scaling) const;
    static QString renderImageForBoard(const BoardX& b, QSize size);

    virtual QSize sizeHint() const;

    void getStoredMove(Square &from, Square &to);
    QRect totalRect() const;

    BoardViewAction moveActionFromModifier(Qt::KeyboardModifiers modifiers) const;
    bool getBrushMode() const;
    void setBrushMode(bool brushMode);

    QSize themeSize() const;
    void setShowAttacks(const Color &showAttacks);
    void setShowUnderProtection(const Color &showUnderProtection);

    Move getBestGuess() const;
    void setBestGuess(const Move &bestGuess);
    void setVariations(const QList<Move>& variations);

    /** Reconfigure current theme. */
    void configure(bool allowErrorMessage=false);

public slots:

    /** Flips/unflips board. */
    void setFlipped(bool flipped);
    /** Start a configure with error messages enabled */
    void reconfigure();
    /** Flips/unflips board. */
    void flip();
    /** Enable / Disable Board for move entry. */
    void setEnabled(bool enabled);
    /** Disable / Enable Board for move entry. */
    void setDisabled(bool disabled);
    /** Show FICS premove */
    void setStoredMove(chessx::Square from, chessx::Square to);

signals:
    /** User clicked source and destination squares */
    void moveMade(chessx::Square from, chessx::Square to, int button);
    /** User requests an evaluation from the current position with the piece @p from replaced at @p to */
    void evalRequest(chessx::Square from, chessx::Square to);
    void evalMove(chessx::Square from, chessx::Square to);
    void evalModeDone();
    /** User dragged and dropped a piece holding Control */
    void copyPiece(chessx::Square from, chessx::Square to);
    /** User dragged and dropped a piece holding Control */
    void invalidMove(chessx::Square from);
    /** User clicked square */
    void clicked(chessx::Square square, int button, QPoint pos, chessx::Square from);
    /** User moved mouse wheel. */
    void wheelScrolled(int dir);
    /** Indicate that a piece was dropped to the board */
    void pieceDropped(chessx::Square to, Piece p);
    void actionHint(const QString&);
    void signalFlipped(bool oldState, bool newState);
    void signalDropEvent(QDropEvent*);
    void signalGamesDropped(QDropEvent*);

protected:
    /** Redraws whole board if necessary. */
    virtual void paintEvent(QPaintEvent*);
    /** Automatically resizes pieces and redisplays board. */
    virtual void resizeEvent(QResizeEvent*);
    /** Handle mouse events */
    virtual void mousePressEvent(QMouseEvent* e);
    /** Handle mouse events */
    virtual void mouseMoveEvent(QMouseEvent* e);
    /** Handle mouse events */
    virtual void mouseReleaseEvent(QMouseEvent* e);
    /** Handle mouse wheel events */
    virtual void wheelEvent(QWheelEvent* e);
    virtual void keyPressEvent(QKeyEvent *);
    virtual void keyReleaseEvent(QKeyEvent *);
#if QT_VERSION < 0x060000
    virtual void enterEvent(QEvent *event);
#else
    virtual void enterEvent(QEnterEvent *event);
#endif
    virtual void leaveEvent(QEvent *event);
    void handleMouseMoveEvent(QMouseEvent *event);

protected: //Drag'n'Drop Support
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);

    void checkCursor(Qt::KeyboardModifiers modifiers);
protected slots:
    void showThreat(Guess::Result sm, BoardX b);
private:
    /** Resizes pieces for new board size. */
    void resizeBoard(QSize size);
    /** Calculate size and position of square */
    QRect squareRect(Square s) const;
    /** Calculate size and position of a coordinate indicator in vertical direction */
    QRect coordinateRectVertical(int n) const;
    /** Calculate size and position of a coordinate indicator in horizontal direction */
    QRect coordinateRectHorizontal(int n) const;
    /** @return square at given position */
    Square squareAt(const QPoint& p) const;
    /** Selects given square. Previously selected square is unselected automatically. */
    void selectSquare(Square s);
    /** Unselects given square. */
    void unselectSquare();
    /** Check if piece at square @p square can be dragged */
    bool canDrag(Square s, Qt::KeyboardModifiers mdf) const;
    /** Check if piece at square @p square can be dropped */
    bool canDrop(Square s) const;
    /** Highlights the from and to squares of a guessed move. */
    bool showGuess(Square s);
    /** Highlights the from and to squares of a threat. */
    void updateThreat();
    /** Recalculate guess when board is changed */
    void updateGuess(Square s);
    /** Remove the guessed move highlight from the board. */
    void removeGuess();
    /** Catch mouse events */
    bool eventFilter(QObject *obj, QEvent *ev);

    void drawArrow(int square1, int square2, QColor color, int thin = 0);
    void drawHiliteSquare(QPoint pos, BoardTheme::ColorRole role);

    QPoint posFromSquare(int square) const;

    void drawColorRect(QPaintEvent* event, Square square, QColor color, bool plain = false);

    void drawHiliting(QPaintEvent* event);
    void drawSquares(QPaintEvent* event);
    void drawTargets(QPaintEvent* event);
    void drawPieces(QPaintEvent* event);
    void drawCheck(QPaintEvent* event);
    void drawAttacks(QPaintEvent *event);
    void drawUnderProtection(QPaintEvent *event);
    void drawMoveIndicator(QPaintEvent* event);
    void drawDraggedPieces(QPaintEvent* event);
    void drawCoordinates(QPaintEvent* event);

    void drawSquareAnnotations(QPaintEvent* event);
    void drawSquareAnnotation(QPaintEvent* event, QString annotation);
    void drawArrowAnnotations(QPaintEvent* event);
    void drawArrowAnnotation(QPaintEvent* event, QString annotation);

    void startToDrag(QMouseEvent *event, Square s);

    BoardX m_board;
    BoardTheme m_theme;
    bool m_flipped;
    bool m_showFrame;
    int m_showCurrentMove;
    bool m_guessMove;
    bool m_showThreat;
    bool m_showTargets;
    bool m_brushMode;
    Square m_selectedSquare;
    Square m_hoverSquare;
    Square m_hiFrom;
    Square m_hiTo;
    Square m_currentFrom;
    Square m_currentTo;
    Square m_storedFrom;
    Square m_storedTo;
    Square m_dragStartSquare;
    Square m_alertSquare;
    QList<Square> m_targets;
    bool m_atLineEnd;
    int m_flags;
    bool m_coordinates;
    Piece m_dragged;
    QPoint m_dragStart;
    QPoint m_dragPoint;
    bool m_clickUsed;
    int m_wheelCurrentDelta;
    int m_minDeltaWheel;
    Guess::MoveList m_moveList;
    unsigned int m_moveListCurrent;
    bool m_showMoveIndicator;
    int m_showMoveIndicatorMode;
    QPointer<QObject> m_DbIndex;
    ThreadedGuess m_threatGuess;
    Move m_bestGuess;
    QList<Move> m_variations;
    Color m_showAttacks;
    Color m_showUnderProtection;
    QMouseEvent* lastMoveEvent;
};

class BoardViewMimeData : public QMimeData
{
    Q_OBJECT

public:
    Piece m_piece;
};

#endif

