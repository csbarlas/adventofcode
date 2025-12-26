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
            sum += calcMaxJoltage(bank: bank, maxLength: 2)
        }
        return sum
    }
    
    func part2() async throws -> Int {
        var sum = 0
        for bank in banks {
            sum += calcMaxJoltage(bank: bank, maxLength: 12)
        }
        return sum
    }
    
    private func calcMaxJoltage(bank: String, maxLength: Int) -> Int {
        var joltages = bank
        
        while joltages.count > maxLength {
            var victim = joltages.startIndex
            
            for index in joltages.indices {
                guard let joltage = joltages[index].wholeNumberValue,
                      let min = joltages[victim].wholeNumberValue else { return -1 }
                
                if joltage < min {
                    victim = index
                } else if joltage > min {
                    break
                }
            }
            
            joltages.remove(at: victim)
        }
        
        return Int(joltages) ?? -1
    }
}
