//
//  Day02.swift
//  AdventOfCode
//
//  Created by Chris Barlas on 12/20/25.
//

struct Day02: AdventDay {
    private struct IdRange {
        var lower, upper: String
    }
    
    var data: String
    
    private var items: [IdRange] {
        data.components(separatedBy: ",").compactMap {
            let tmp = $0.components(separatedBy: "-")
            if let lower = tmp.first, let upper = tmp.last {
                return IdRange(lower: lower, upper: upper)
            }
            
            return nil
        }
    }
    
    func part1() async throws -> Int {
        invalidIdAggregator(isInvalid: isInvalidPartOne(_:))
    }
    
    func part2() async throws -> Int {
        invalidIdAggregator(isInvalid: isInvalidPartTwo(_:))
    }
    
    private func invalidIdAggregator(isInvalid: (Int) -> Bool) -> Int {
        var sum = 0
        for range in items {
            if let lower = Int(range.lower.trimmingCharacters(in: .whitespacesAndNewlines)),
               let upper = Int(range.upper.trimmingCharacters(in: .whitespacesAndNewlines)) {
                for number in lower...upper {
                    if isInvalid(number) {
                        sum += number
                    }
                }
            }
        }
        return sum
    }
    
    private func isInvalidPartOne(_ id: Int) -> Bool {
        let id = String(id)
        for i in id.indices.prefix(id.count / 2) {
            let pattern = id[...i]
            let matches = id.ranges(of: pattern)
            // first part tests for 2 matches (constraint #1)
            // second part tests that the 2 matches comprise the whole string
            if matches.count == 2 && (matches.count * pattern.count == id.count) {
                return true
            }
        }
        
        return false
    }
    
    private func isInvalidPartTwo(_ id: Int) -> Bool {
        let id = String(id)
        for i in id.indices.prefix(id.count / 2) {
            let pattern = id[...i]
            let matches = id.ranges(of: pattern)
            // first part tests for at least 2 matches (constraint #1)
            // second part tests that the matches comprise the whole string
            if matches.count >= 2 && (matches.count * pattern.count == id.count) {
                return true
            }
        }
        
        return false
    }
}
