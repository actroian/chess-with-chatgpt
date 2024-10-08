#include "L3.h"
#include "player.h"

using namespace std;

L3::L3(bool isWhite) : L2{isWhite} {}

unique_ptr<Move> L3::chooseMove(unique_ptr<Board>& b, Player* p2) {
    vector<unique_ptr<Move>> avoid = avoidCaptureMoves(b, p2);
    int randomMove;

    if (!avoid.empty()) {
        randomMove = rand() % avoid.size();
        return std::move(avoid[randomMove]);
    }
    return L2::chooseMove(b, p2);
}

vector<unique_ptr<Move>> L3::avoidCaptureMoves(unique_ptr<Board>& b, Player* p2) {
    vector<unique_ptr<Move>> avoid;

    vector<pair<int, int>> myPos = getMyPiecePositions(b, isWhite);
    vector<unique_ptr<Move>> otherPlayersMoves = p2->possibleMoves(b, p2, false);

    // Vector to store capture positions
    vector<pair<int, int>> capturePositions;

    // Fill capturePositions with positions from otherPlayersMoves
    for (const auto& move : otherPlayersMoves) {
        for (const auto& pos : myPos) {
            if(move != nullptr){
                if (move->end.first == pos.first && move->end.second == pos.second) {
                    // Check if the position is already in capturePositions
                    bool found = false;
                    for (const auto& capturePos : capturePositions) {
                        if (capturePos == make_pair(pos.first, pos.second)) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        capturePositions.push_back(make_pair(pos.first, pos.second));
                    }
                }
            }
        }
    }

    vector<unique_ptr<Move>> myMoves = possibleMoves(b, p2, false);
    vector<unique_ptr<Move>> possibleAvoidMoves;
    for (auto& move : myMoves) {
        if(move != nullptr){
            for (const auto& capturePos : capturePositions) {
                if (move->start.first == capturePos.first && move->start.second == capturePos.second) {
                    possibleAvoidMoves.push_back(std::move(move));
                    break;
                }
            }
        }
    }
    for (auto& posMove: possibleAvoidMoves) {
        bool isSafe = true;
        for (const auto& oppMove : otherPlayersMoves) {
            if(oppMove && posMove->end == oppMove->end) {
                isSafe = false;
                break;
            }
        }
        if (isSafe) {
            avoid.push_back(std::move(posMove));
        }
    }

    return avoid;
}