#include <vector>
#include <unordered_map>
#include <functional>
#include <future>
namespace reduceBeginner {
  template 
  <
    typename Key,
    typename Val,
    typename IKey,
    typename IVal,
    typename RKey,
    typename RVal
  >
  class MapReduce {
  public:
    typedef std::vector<std::pair<IKey, IVal>> MapResult;
    typedef std::vector<std::pair<Key, Val>> InputList;
    typedef std::function<MapResult(Key, Val)> Mapper;
    
    typedef std::unordered_map<RKey, RVal> ResultTable;
    typedef std::vector<IVal> ReduceList;
    typedef std::pair<RKey, RVal> ReduceResult;
    typedef std::function<ReduceResult(IKey, ReduceList)> Reducer;

    MapReduce(Mapper mapper, Reducer reducer) : M(mapper), R(reducer) {}

    ResultTable operator()(const InputList& input) {
      
      std::unordered_map<IKey, ReduceList> tempStorage;

      std::vector<std::future<MapResult>> futures;
      for (auto&& pair : input) {
      	futures.push_back(std::async(M, pair.first, pair.second));
      }

      for (auto&& fut : futures) {
        for (auto&& mapresult : fut.get()) {
          tempStorage[mapresult.first].push_back(mapresult.second);
        }
      }
      ResultTable result;
      std::vector<std::future<ReduceResult>> rfutures;
      for (auto&& pair : tempStorage) {
        rfutures.push_back(std::async(R, pair.first, pair.second));
      }
      for (auto&& fut : rfutures) {
      	result.insert(fut.get());
      }
      return result;
    }
  private:
    Mapper M;
    Reducer R;
  };
}
