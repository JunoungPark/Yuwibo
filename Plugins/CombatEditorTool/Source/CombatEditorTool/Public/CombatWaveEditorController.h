#pragma once

class UCombatWaveType;
class ABaseCharacter;
struct FMonsterSpawnData;
struct FWaveData;

class FCombatWaveEditorController : public TSharedFromThis<FCombatWaveEditorController>
{
public:
    FCombatWaveEditorController();
    ~FCombatWaveEditorController();

    void Initialize();

    // DataAsset 접근
    void LoadDataAsset(); // "/Game/..." 경로 사용
    void CreateNewDataAsset();
    void SaveDataAsset();
    UCombatWaveType* GetLoadedAsset() const { return LoadedAsset; }
    TArray<FMonsterSpawnData> GetWaveMonster();
    // 간단한 편집 API (위젯에서 호출)
    int32 AddWave();
    void AddMonsterToWave(const FMonsterSpawnData Monster);

    void SetSelectWave(TSharedPtr<FWaveData> SelectedItem);
    // Preview control
    void SpawnPreview();
    void ClearPreview();

private:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient)
    TArray<ABaseCharacter*> SpawnMonsters;

    UCombatWaveType* LoadedAsset = nullptr; // 에디터 전용 포인터
    int32 SelectWave = 0; // 에디터 전용 포인터
};