#ifndef HGCalTower_SA_h
#define HGCalTower_SA_h

#include <cstdint>
#include <vector>

// Namespace taken from HGCalTriggerCell_SA.h
namespace l1thgcfirmware {

  // One tower (pTT) produced by the Stage-1 tower emulation for one bunch crossing.
  class HGCalTower {
  public:
    // Which part of the calorimeter the tower sum comes from.
    enum class Detector : uint8_t { CE_E = 0, CE_H = 1 };

    // Value of tmuxLink_ / tmuxPosition_ before they have been assigned.
    static constexpr int kUnassigned = -1;

    HGCalTower() : HGCalTower(0, 0, 0, Detector::CE_E, 0, 0) {}

    HGCalTower(unsigned int board,
               unsigned int eta,
               unsigned int phi,
               Detector detector,
               unsigned int bx,
               uint8_t energy)
        : board_(board),
          eta_(eta),
          phi_(phi),
          detector_(detector),
          bx_(bx),
          energy_(energy),
          tmuxLink_(kUnassigned),
          tmuxPosition_(kUnassigned) {}

    ~HGCalTower() {}

    // Setters
    void setBoard(const unsigned int board) { board_ = board; }
    void setEta(const unsigned int eta) { eta_ = eta; }
    void setPhi(const unsigned int phi) { phi_ = phi; }
    void setDetector(const Detector detector) { detector_ = detector; }
    void setBx(const unsigned int bx) { bx_ = bx; }
    void setEnergy(const uint8_t energy) { energy_ = energy; }
    void setTmuxLink(const int tmuxLink) { tmuxLink_ = tmuxLink; }
    void setTmuxPosition(const int tmuxPosition) { tmuxPosition_ = tmuxPosition; }

    // Getters
    unsigned int board() const { return board_; }
    unsigned int eta() const { return eta_; }
    unsigned int phi() const { return phi_; }
    Detector detector() const { return detector_; }
    unsigned int bx() const { return bx_; }
    // 8-bit 4E4M code. Note: uint8_t prints as a character, cast to unsigned int before printing.
    uint8_t energy() const { return energy_; }
    int tmuxLink() const { return tmuxLink_; }
    int tmuxPosition() const { return tmuxPosition_; }

    bool tmuxAssigned() const { return tmuxLink_ != kUnassigned && tmuxPosition_ != kUnassigned; }

    // Operators
    bool operator==(const HGCalTower& rhs) const {
      return board_ == rhs.board_ && eta_ == rhs.eta_ && phi_ == rhs.phi_ && detector_ == rhs.detector_ &&
             bx_ == rhs.bx_ && energy_ == rhs.energy_ && tmuxLink_ == rhs.tmuxLink_ &&
             tmuxPosition_ == rhs.tmuxPosition_;
    }
    bool operator!=(const HGCalTower& rhs) const { return !(*this == rhs); }

  private:
    unsigned int board_;
    unsigned int eta_;
    unsigned int phi_;
    Detector detector_;
    unsigned int bx_;
    uint8_t energy_;
    int tmuxLink_;
    int tmuxPosition_;
  };

  // All towers of a file (or of any subset), over any number of BXs.
  class HGCalTowerCollection {
  public:
    HGCalTowerCollection() {}
    ~HGCalTowerCollection() {}

    void add(const HGCalTower& tower) { towers_.push_back(tower); }
    void clear() { towers_.clear(); }

    std::size_t size() const { return towers_.size(); }
    bool empty() const { return towers_.empty(); }
    // Throws std::out_of_range if i >= size().
    const HGCalTower& at(const std::size_t i) const { return towers_.at(i); }
    HGCalTower& at(const std::size_t i) { return towers_.at(i); }
    const std::vector<HGCalTower>& towers() const { return towers_; }

    // Copies of the towers belonging to one BX, in the order they were added.
    std::vector<HGCalTower> towersInBX(const unsigned int bx) const {
      std::vector<HGCalTower> selected;
      for (const auto& tower : towers_) {
        if (tower.bx() == bx)
          selected.push_back(tower);
      }
      return selected;
    }

    bool operator==(const HGCalTowerCollection& rhs) const { return towers_ == rhs.towers_; }
    bool operator!=(const HGCalTowerCollection& rhs) const { return !(*this == rhs); }

  private:
    std::vector<HGCalTower> towers_;
  };

}  // namespace l1thgcfirmware

#endif