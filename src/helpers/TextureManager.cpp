//
// Created by olbai on 02/07/2025.
//

#include "helpers/TextureManager.hpp"

const std::array<std::string, 12> TextureManager::chessPieceIds = {
    "Chess_plt60", "Chess_nlt60", "Chess_blt60",
    "Chess_rlt60", "Chess_qlt60", "Chess_klt60",
    "Chess_pdt60", "Chess_ndt60", "Chess_bdt60",
    "Chess_rdt60", "Chess_qdt60", "Chess_kdt60",
};

TextureManager::TextureManager() {
    for (const std::string &pieceId : chessPieceIds) {
        sf::Texture texture;
        if (!texture.loadFromFile("../resources/pieces/" + pieceId + ".png")) {
            std::cerr << "Failed to load Texture: " << pieceId << std::endl;
        }

        pieceTextures.insert({pieceId, texture});
    }
}

const sf::Texture &TextureManager::getTexture(const std::string& id) const {return pieceTextures.at(id);}