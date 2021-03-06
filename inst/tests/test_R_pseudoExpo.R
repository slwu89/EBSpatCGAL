require(EBSpatCGAL)

#del2 <- Delaunay()

#insert(del2,matrix(runif(200,-350,350),ncol=2))

# lc <- GNZCache(del2~Del2(Th[1]*(l<=20)+Th[2]*(20<l & l<=80))+Del1(Th2*(a<20)),
# 			runs=10000L,
# 			domain=c(-300,-300,300,300)
# 		)
# formula(lc,expression(first=exp(-(.V)),second=-(.V)))
# run(lc,Th=c(0,0),Th2=3,single=-log(1/3600))

if(FALSE) {
	pseudo <- Pseudo(del2~Del2(Th[1]*(l<=20)+Th[2]*(20<l & l<=80)), 
		runs=10000L,
		domain=Domain(c(-300,-300),c(300,300)),
		expo=TRUE
	)

	run(pseudo,Single=2,Th=c(2,4))
} else { # with Horizon fixed to 0
	pseudo2 <- Pseudo(del2~Del2(Th[1]*(l<=20)+Th[2]*(20<l & l<=80)+Th[3]*(80<l)), 
		runs=10000L,
		domain=Domain(c(-300,-300),c(300,300)),
		expo=TRUE
	)

	run(pseudo2,Single=2,Th=c(2,4,0),fixed=4)
}