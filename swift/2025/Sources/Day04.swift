//
//  Day04.swift
//  AdventOfCode
//
//  Created by Chris Barlas on 12/28/25.
//

enum MapEntity: Character {
    case free = "."
    case paperRoll = "@"
}

struct GridCursor {
    enum CompassDirection {
        case north
        case northEast
        case east
        case southEast
        case south
        case southWest
        case west
        case northWest
    }

    private let map: [[MapEntity]]
    private var x, y: Int

    init(map: [[MapEntity]], x: Int, y: Int) {
        self.map = map
        self.x = x
        self.y = y
    }

    func read() -> MapEntity {
        return map[y][x]
    }

    func read(directions: [CompassDirection], perform: (MapEntity) -> Void) {
        for direction in directions {
            var temp = self
            var final: MapEntity?
            switch direction {
            case .north:
                if let result = temp.up() {
                    final = result
                }
            case .northEast:
                if let _ = temp.up(), let result = temp.right() {
                    final = result
                }
            case .east:
                if let result = temp.right() {
                    final = result
                }
            case .southEast:
                if let _ = temp.right(), let result = temp.down() {
                    final = result
                }
            case .south:
                if let result = temp.down() {
                    final = result
                }
            case .southWest:
                if let _ = temp.down(), let result = temp.left() {
                    final = result
                }
            case .west:
                if let result = temp.left() {
                    final = result
                }
            case .northWest:
                if let _ = temp.left(), let result = temp.up() {
                    final = result
                }
            }
            if let final {
                perform(final)
            }
        }
    }

    private mutating func up() -> MapEntity? {
        if y > 0 {
            y -= 1
            return map[y][x]
        }

        return nil
    }

    private mutating func down() -> MapEntity? {
        if y < map.count - 1 {
            y += 1
            return map[y][x]
        }

        return nil
    }

    private mutating func left() -> MapEntity? {
        if x > 0 {
            x -= 1
            return map[y][x]
        }

        return nil
    }

    private mutating func right() -> MapEntity? {
        if x < map[y].count - 1 {
            x += 1
            return map[y][x]
        }

        return nil
    }
}

struct Day04: AdventDay {

    var data: String

    private var forkliftMap: [[MapEntity]] {
        var map = [[MapEntity]]()
        for line in data.components(separatedBy: "\n") {
            if line.isEmpty { continue }
            let temp = line.map { item in
                switch item {
                case MapEntity.paperRoll.rawValue:
                    return MapEntity.paperRoll
                default:
                    return MapEntity.free
                }
            }
            map.append(temp)
        }
        return map
    }

    func part1() async throws -> Int {
        var accessibleRolls = 0

        for lineIndex in forkliftMap.indices {
            let line = forkliftMap[lineIndex]
            for itemIndex in line.indices {
                let cursor = GridCursor(map: forkliftMap, x: itemIndex, y: lineIndex)
                
                if cursor.read() == .paperRoll && adjacentPaperRolls(cursor: cursor) < 4 {
                    accessibleRolls += 1
                }
            }
        }
        return accessibleRolls
    }

    func part2() async throws -> Int {
        return 0
    }

    private func adjacentPaperRolls(cursor: GridCursor) -> Int {
        var paperRolls = 0
        
        cursor.read(directions: [.north, .northEast, .east, .southEast, .south, .southWest, .west, .northWest]) { paperRolls += $0 == .paperRoll ? 1 : 0 }
        
        return paperRolls
    }

}
