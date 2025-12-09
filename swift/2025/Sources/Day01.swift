import Algorithms

fileprivate enum Direction {
    case left, right
}

fileprivate struct SequenceInstruction {
    let direction: Direction
    let distance: Int
}

struct Day01: AdventDay {
    // Save your data in a corresponding text file in the `Data` directory.
    var data: String
    
    fileprivate var instructions: [SequenceInstruction] {
        data.split(separator: "\n").compactMap { line in
            if let direction: Direction = (line.first == "R") ? .right : .left,
               let distance: Int = Int(line.dropFirst()) {
                return SequenceInstruction(direction: direction, distance: distance)
            }
            
            return nil
        }
    }

    func part1() -> Any {
        var dial = 50
        var zeros = 0
        
        for instruction in instructions {
            dial += (instruction.direction == .left ? -1 : 1) * instruction.distance
            
            // edge case where dial is at 0 and we go negative
            while dial < 0 {
                dial += 100
            }
            
            // edge case where dial is at 99 and we go over
            if dial > 99 {
                dial %= 100
            }
            
            if dial == 0 {
                zeros += 1
            }
        }
        
        return zeros
    }

    func part2() -> Any {
        var dial = 50
        var zeroVisits = 0
        
        for instruction in instructions {
            let preDial = dial
            dial += (instruction.direction == .left ? -1 : 1) * instruction.distance
            
            // edge case where dial is at 0 and we go negative
            // problem is here somewhere
            if dial < 0 {
                var countZeros = true
                if preDial == 0 {
                    zeroVisits += instruction.distance / 100
                    countZeros = false
                }
                
                while dial < 0 {
                    dial += 100
                    if countZeros { zeroVisits += 1 }
                }
            }
            
            // edge case where dial is at 99 and we go over
            if dial > 99 {
                zeroVisits += dial / 100
                dial %= 100
                continue
            }
            
            if dial == 0 {
                zeroVisits += 1
            }
        }
        
        return zeroVisits
    }
}
