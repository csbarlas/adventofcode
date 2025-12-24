//
//  Day02.swift
//  AdventOfCode
//
//  Created by Chris Barlas on 12/20/25.
//

// TODO: Utilize Swift Concurrency to speed up compute time

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
        invalidIdAggregator(isInvalid: isInvalid(_:partTwo:), partTwo: false)
    }
    
    func part2() async throws -> Int {
        invalidIdAggregator(isInvalid: isInvalid(_:partTwo:), partTwo: true)
    }
    
    private func invalidIdAggregator(isInvalid: (Int, Bool) -> Bool, partTwo: Bool = false) -> Int {
        var sum = 0
        for range in items {
            if let lower = Int(range.lower.trimmingCharacters(in: .whitespacesAndNewlines)),
               let upper = Int(range.upper.trimmingCharacters(in: .whitespacesAndNewlines)) {
                for number in lower...upper {
                    if isInvalid(number, partTwo) {
                        sum += number
                    }
                }
            }
        }
        return sum
    }
    
    private func isInvalid(_ id: Int, partTwo: Bool = false) -> Bool {
        let id = String(id)
        for i in id.indices.prefix(id.count / 2) {
            let pattern = id[...i]
            let matches = id.ranges(of: pattern)
            // first part tests for 2 matches (constraint #1)
            // second part tests that the 2 matches comprise the whole string
            if (partTwo ? matches.count >= 2 : matches.count == 2) && (matches.count * pattern.count == id.count) {
                return true
            }
        }
        
        return false
    }
}
