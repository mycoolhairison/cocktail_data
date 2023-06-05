//  ContentView.swift
//  Cocktail Selector
//
//  Created by Michael Harrison on 5/23/23.
//
//  Interface which allows user to filter cocktail list according to some
//  basic criteria.  Due to hacky spacing, best viewed on iPhone 14 Pro.

import SwiftUI

struct ContentView: View {
    //presort cocktailData by Rating, then Name
    private var listofCocktails = cocktailData.sorted(by: {$0[1]>$1[1] || ($0[1]==$1[1] && $0[2]>$1[2]) || ($0[1]==$1[1] && $0[2]==$1[2] && $0[0]<$1[0])})
    
    @State private var isFruity = false
    @State private var isEasy = false
    @State private var isHoliday = false
    @State private var isClassic = true
    @State var searchText = ""
    @State var seasonIndex = 0
    @State var spiritOneIndex = 0
    let seasons = ["All","Winter","Spring","Summer","Fall"]
    let spirits = ["(select spirit)", "Brandy", "Champagne", "Gin", "Liqueurs", "Mezcal", "Rum", "Sangria", "Tequila", "Vodka", "Whiskey"]
    
    var body: some View {
        NavigationView {
            List {
                //This Section contains various filtering options
                Section {
                    HStack {
                        Spacer()
                        Menu {
                            Picker("", selection: $spiritOneIndex) {
                                ForEach(0 ..< spirits.count, id: \.self) {
                                    Text("\(spirits[$0])")
                                }
                            }
                        }
                        label: {
                            Label(spirits[spiritOneIndex], systemImage: "chevron.down")
                        }
                        .fontWeight(.bold)
                        .frame(width: 180, height: 30)
                        .foregroundColor(Color.black)
                        .background(Color.teal)
                        .cornerRadius(25)
                        .alignmentGuide(.listRowSeparatorLeading) { $0[.leading]-87.5
                        }
                        Spacer()
                    }
                    HStack {
                        Toggle("Easy", isOn: $isEasy)
                            .toggleStyle(.button)
                            .tint(.mint)
                            .foregroundColor(.black)
                            .overlay(
                                RoundedRectangle(cornerRadius: 25)
                                    .stroke(Color.black, lineWidth: 2)
                            )
                            .border(.black)
                            .cornerRadius(20)
                        Toggle("Fruity", isOn: $isFruity)
                            .toggleStyle(.button)
                            .tint(.mint)
                            .foregroundColor(.black)
                            .overlay(
                                RoundedRectangle(cornerRadius: 25)
                                    .stroke(Color.black, lineWidth: 2)
                            )
                            .border(.black)
                            .cornerRadius(20)
                        Toggle("Classic", isOn: $isClassic)
                            .toggleStyle(.button)
                            .tint(.mint)
                            .foregroundColor(.black)
                            .overlay(
                                RoundedRectangle(cornerRadius: 25)
                                    .stroke(Color.black, lineWidth: 2)
                            )
                            .border(.black)
                            .cornerRadius(20)
                        Toggle("Holiday", isOn: $isHoliday)
                            .toggleStyle(.button)
                            .tint(.mint)
                            .foregroundColor(.black)
                            .overlay(
                                RoundedRectangle(cornerRadius: 25)
                                    .stroke(Color.black, lineWidth: 2)
                            )
                            .border(.black)
                            .cornerRadius(20)
                    }
                    .padding(1)
                    .alignmentGuide(.listRowSeparatorLeading) { $0[.leading]-20 }
                    
                    Picker("", selection: $seasonIndex) {
                        ForEach(0 ..< seasons.count, id: \.self) {
                            Text("\(seasons[$0])")
                        }
                    }
                    .pickerStyle(SegmentedPickerStyle())
                    .padding(4)
                }
                
                //This section lists all cocktails which satisfy filtering constraints
                ForEach(cocktail, id: \.self) { cocktail in
                    HStack {
                        VStack(alignment: .leading, spacing: -8) {
                            Text(cocktail[0].capitalized)
                                .fontWeight(.bold)
                                .font(.system(size:18))
                            HStack {
                                GeometryReader { geometry in
                                    ZStack(alignment: .leading) {
                                    Rectangle()
                                    Rectangle()
                                        .fill(Color.orange)
                                        .frame(width: max(0.0,84.5 * ((Double(cocktail[1]) ?? 0)/5.0)))
                                    }
                                }
                                .mask {
                                    HStack(spacing: -1) {
                                        Image(systemName: "star.fill")
                                        Image(systemName: "star.fill")
                                        Image(systemName: "star.fill")
                                        Image(systemName: "star.fill")
                                        Image(systemName: "star.fill")
                                        Text(" (" + cocktail[2] + ")")
                                    }
                                    .imageScale(.small)
                                }
                                Spacer(minLength: 165)
                                Link(destination: URL(string: "https://www.seriouseats.com/" + cocktail[3])!) {
                                    Image(systemName: "wineglass")
                                        .frame(width: 25, height: 25)
                                        .foregroundColor(Color.black)
                                        .background(Color.teal)
                                        .cornerRadius(7)
                                }
                            }
                        }
                    }
                    .padding([.leading, .trailing, .top], 6)
                    .listRowSeparatorTint(.cyan)
                    .alignmentGuide(.listRowSeparatorLeading) { $0[.leading]-20 }
                }
            }
            .searchable(text: $searchText)
            .navigationTitle("Cocktail Selector")
        }
    }
    
    //Delete parentheticals from the cocktail name
    func getShortName(name : String)-> String {
        let firstPar = name.firstIndex(of: "(") ?? name.endIndex
        let shortName = name[..<firstPar]
        return String(shortName)
    }
    
    //  Filtering method
    //  Cocktail data is in the following format:
    //  [name,rating,numberOfRatings,link,keywords,tags,extraKeywords]
    //  The latter three are useful for different search criteria
    var cocktail: [[String]] {
        let lcSpirits = listofCocktails.map { [getShortName(name: $0[0]).lowercased(),$0[1],$0[2],$0[3],$0[4].lowercased(),$0[5].lowercased(),$0[6].lowercased()] }
        
        return lcSpirits.filter {
            (searchText == "" || $0[0].contains(searchText.lowercased()) ||
                $0[6].contains(searchText.lowercased())
            )
            &&
            (spiritOneIndex==0 || $0[4].contains(spirits[spiritOneIndex].lowercased()) || $0[5].contains(spirits[spiritOneIndex].lowercased()) || $0[6].contains(spirits[spiritOneIndex].lowercased())
            )
            &&
            (seasonIndex==0 || $0[4].contains(seasons[seasonIndex].lowercased()) || $0[5].contains(seasons[seasonIndex].lowercased()) || $0[6].contains(seasons[seasonIndex].lowercased())
            )
            &&
            (isFruity == false || $0[5].lowercased().contains("fruit"))
            &&
            (isEasy == false || $0[6].contains("easy"))
            &&
            (isClassic == false || $0[6].contains("classic"))
            &&
            (isHoliday == false || $0[4].contains("holiday") ||
                $0[5].contains("holiday") || $0[6].contains("holiday")
            )
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
