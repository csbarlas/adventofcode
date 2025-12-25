//
//  Day03.swift
//  AdventOfCode
//
//  Created by Chris Barlas on 12/24/25.
//

struct Day03: AdventDay {
    var data: String
    
    var banks: [String] {
        data.components(separatedBy: "\n").compactMap {
            let line = $0.trimmingCharacters(in: .whitespacesAndNewlines)
            if line.isEmpty { return nil }
            else { return line }
        }
    }
    
    func part1() async throws -> Int {
        var sum = 0
        for bank in banks {
            sum += calcMaxJoltage(bank: bank)
        }
        return sum
    }
    
    func part2() async throws -> Int {
        return 0
    }
    
    private func calcMaxJoltage(bank: String) -> Int {
        var tensDigitIdx = bank.startIndex
        var onesDigitIdx = bank.index(after: bank.startIndex)
        guard let num = bank.indexesToInt(tens: tensDigitIdx, ones: onesDigitIdx) else { return 0 }
        var currentMax = num
        var index = bank.index(after: onesDigitIdx)
        while index < bank.endIndex {
            var newTens = tensDigitIdx
            var newOnes = onesDigitIdx
            var tempMax = currentMax
            var windowDidJump = false
            // Options, be greedy:
            // 1. [new digit][new digit + 1] is bigger than current
            // 2. [current ones][new digit] is bigger than current
            // 2. [current tens][new digit] is bigger than current
            // 3. no change
            
            let splitWindow = bank.indexesToInt(tens: tensDigitIdx, ones: index) ?? 0
            if splitWindow > tempMax {
                tempMax = splitWindow
                newOnes = index
            }
            
            let windowShift = bank.indexesToInt(tens: onesDigitIdx, ones: index) ?? 0
            if windowShift > tempMax {
                tempMax = windowShift
                newTens = onesDigitIdx
                newOnes = index
            }
            
            if index != bank.index(before: bank.endIndex) {
                let peekAheadOnes = bank.index(after: index)
                let windowJump = bank.indexesToInt(tens: index, ones: peekAheadOnes) ?? 0
                if windowJump > tempMax {
                    tempMax = windowJump
                    newTens = index
                    newOnes = peekAheadOnes
                    windowDidJump = true
                }
            }
            
            // Commit any larger numbers that were found
            currentMax = tempMax
            tensDigitIdx = newTens
            onesDigitIdx = newOnes
            
            // Move forward pointer
            index = bank.index(after: index)
            if windowDidJump { index = bank.index(after: index) }
        }
        
        return currentMax
    }
}

private extension String {
    func indexToInt(_ index: Self.Index) -> Int? {
        Int(String(self[index]))
    }
    
    func indexesToInt(tens: Self.Index, ones: Self.Index) -> Int? {
        guard let tens = self.indexToInt(tens),
              let ones = self.indexToInt(ones) else { return nil }
        
        return (tens * 10) + ones
    }
}
