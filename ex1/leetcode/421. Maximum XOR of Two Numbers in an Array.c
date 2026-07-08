int findMaximumXOR(int* nums, int numsSize){
    int i,j,max=0;
    for(i=0;i<numsSize;i++){
        for(j=0;j<numsSize;j++){
            if((nums[i]^nums[j])>max){
                max=nums[i]^nums[j];
            }
        }
    }
    return max;
}
